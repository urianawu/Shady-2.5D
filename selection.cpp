#include "base.h"
#include "commands.h"
#include <qDebug>
Selectable::Selectable(bool isUI):Renderable(isUI){
    _isDraggable = false;
    pRef = 0;
    Session::get()->selectionMan()->insert(this);
}

Selectable::~Selectable(){
     Session::get()->selectionMan()->remove(this);
}

bool Selectable::isTheSelected() const {
    return this == Session::get()->selectionMan()->getTheSelected();
}

bool Selectable::isInSelection() const{
    return Session::get()->selectionMan()->isInSelection((Selectable_p)this);
}

SelectionManager::SelectionManager(){
    _count = 0;
    _theSelected = 0;
    _lastSelected = 0;
}

void SelectionManager::insert(Selectable_p pS){
    if (pS==0) return;
    _selectables[_count] = pS;
    pS->_name = _count;
    _count++;
}

void SelectionManager::remove(Selectable_p pS){
    if (pS==0) return;
    _selectables.erase(pS->_name);
}

Selectable_p SelectionManager::get(int iname){
    SelectableMap::const_iterator it = _selectables.find(iname);
    if (it == _selectables.end())
        return 0;
    return it->second;
}

bool SelectionManager::isInSelection(Selectable_p pS){
    return _selection.find(pS) != _selection.end();
}


void SelectionManager::startSelect(Selectable_p pObj, const Click & click)
{
    isSelect = click.is(Click::DOWN);
    _theSelected = pObj;
    _click0 = click;
    //qDebug()<<pObj->name();

    if (pObj)
        pObj->onDown();
    else
        return;

    if (click.isCtrl){
        if (isSelect)
            _selection.insert(pObj);
        else
            _selection.erase(pObj);
    }else
        _selection.clear();
}

void SelectionManager::stopSelect(const Click& click){
    _lastSelected = _theSelected;
    _click1 = click;
    if (!_theSelected)
        return;

    if (_theSelected->isDraggable()){
        Vec2 t = _click1.P - _click0.P;
        if ( t.normsqr() > 0.0001)
            Session::get()->exec(new Drag((Draggable_p)_theSelected, t));
    }

    _theSelected->onUp();
    _theSelected = 0;
}

void SelectionManager::reset(){
    _selectables.clear();
    _count = 0;
}

bool SelectionManager::dragTheSelected(const Point& t, int button){

    if (!_theSelected || !_theSelected->isDraggable())
        return false;
    Draggable_p dragged = ((Draggable_p)_theSelected);
    if (dragged->isLocked)
        return false;
    dragged->drag(t, button);

    return true;
}

