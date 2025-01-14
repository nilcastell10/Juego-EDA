#include "Player.hh"
#include <queue>
#include <vector>
#include <set>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Verdoso


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

bool pintat(Pos pos){
  if(pos_ok(pos)){
    Cell cel = cell(pos);
    if(cel.id!=-1)return true;
    else{
      if(cel.owner!=me())return false;
      else return true;
    }
  }
  else return true;
}

bool unitat(Pos pos){
  if(pos_ok(pos)){
    Cell cel = cell(pos);
    if(cel.id!=-1)return true;
    else return false;
  }
  else return true;
}

bool unitmeva(Pos pos){
  if(pos_ok(pos)){
    Cell cel = cell(pos);
    if(cel.id != -1){
      Unit u = unit(cel.id);
      if(u.player == me())return true;
      else return false;
    }
    return false;
  }
  return true;
}

bool BFSsol(Pos pos){
  bool found=false;
  int cont=0;
  if(pos_ok(pos) and daylight(pos))found=true;
  while(not found){
    ++cont;
    pos+=Left;
    if(daylight(pos))found=true;
  }
  if(cont>7)return true;
  return false;
}

bool rock(Pos pos){
  if(pos_ok(pos)){
    Cell cel = cell(pos);
    if(cel.type == Rock)return true;
    else return false;
  }
  return true;
}

bool sortida(Pos pos, bool p, int dir){
  bool sortim = false;
  int i=0;
  int act;
  while(not sortim and i<5){
    if(i==0)act=dir;
    else if(i==1)act=dir+1;
    else if(i==2)act=dir-1;
    else if(i==3)act=dir+2;
    else act=dir-2;
    if(act>7)act=act-8;
    if(act<0)act=act+8;
    if(p){
      if(not pintat(pos))sortim = true;
      else{
        if(not rock(pos+Dir(act)) and not unitat(pos+Dir(act)))sortim=true;
      }
    }
    else {
      if(not rock(pos+Dir(act)) and not unitmeva(pos+Dir(act)))sortim=true;
    }
    ++i;
  }
  return sortim;
}

void executa(int dir,int id,Pos pos,bool p,bool fet){
  cerr << pos << endl;
  if(pos_ok(pos+Dir(dir)) and not rock(pos+Dir(dir)) and not unitmeva(pos+Dir(dir))and sortida(pos+Dir(dir),p,dir))command(id,Dir(dir));
  else{
    int i=0;
    bool commanded = false;
    while(not commanded and i<4){
      int aux;
      if(i==0)aux=dir+1;
      else if(i==1)aux=dir-1;
      else if(i==2)aux=dir+2;
      else aux=dir-2;
      if(aux<0)aux=aux+8;
      if(aux>7)aux=aux-8;
      if(pos_ok(pos+Dir(aux)) and not rock(pos+Dir(aux)) and not unitmeva(pos+Dir(aux)) and sortida(pos+Dir(aux),p,aux)){
        commanded=true;
        command(id,Dir(aux));
      }
      ++i;
    }
    int aux = dir + 4;
    if(aux<0)aux=aux+8;
    if(aux>7)aux=aux-8;
    if(not commanded and not fet)executa(aux,id,pos,p,true);
    else if(not commanded) command(id,None);
  }
}

void escapa(Pos pos, Pos fin, int id){
    if(pos.i==fin.i and (fin.j-pos.j)<65 and fin.j>pos.j){
        if(not rock(pos+Left) and not unitat(pos+Left))command(id,Left);
        else executa(6,id,pos,false,false);
    }
    else if(pos.i==fin.i and (fin.j-pos.j)>=65 and fin.j>pos.j){
        if(not rock(pos+Right) and not unitat(pos+Right))command(id,Right);
        else executa(2,id,pos,false,false);
    }
    else if(pos.i==fin.i and (pos.j-fin.j)>=65 and fin.j<pos.j){
        if(not rock(pos+Left) and not unitat(pos+Left))command(id,Left);
        else executa(6,id,pos,false,false);
    }
    else if(pos.i==fin.i and (pos.j-fin.j)<65 and fin.j<pos.j){
        if(not rock(pos+Right) and not unitat(pos+Right))command(id,Right);
        else executa(2,id,pos,false,false);
    }
    else if(pos.j==fin.j and fin.i<pos.i){
        if(not rock(pos+Bottom) and not unitat(pos+Bottom))command(id,Bottom);
        else executa(0,id,pos,false,false);
    }
    else if(pos.j==fin.j and fin.i>pos.i){
        if(not rock(pos+Top) and not unitat(pos+Top))command(id,Top);
        else executa(4,id,pos,false,false);
    }
    else if(pos.i<fin.i and (fin.j-pos.j)<65 and fin.j>pos.j){
        if(not rock(pos+TL) and not unitat(pos+TL))command(id,TL);
        else executa(5,id,pos,false,false);
    }
    else if(pos.i<fin.i and (fin.j-pos.j)>=65 and fin.j>pos.j){
        if(not rock(pos+RT) and not unitat(pos+RT))command(id,RT);
        else executa(3,id,pos,false,false);
    }
    else if(pos.i>fin.i and (fin.j-pos.j)<65 and fin.j>pos.j){
        if(not rock(pos+LB) and not unitat(pos+LB))command(id,LB);
        else executa(7,id,pos,false,false);
    }
    else {
        if(not rock(pos+BR) and not unitat(pos+BR))command(id,BR);
        else executa(1,id,pos,false,false);
    }
}

void moua(Pos posicio, Pos fin, bool enemic, int id, bool pio){


    cerr << posicio << endl;
    int disti = fin.i - posicio.i;
    int distj = fin.j - posicio.j;
    int diferencia;
    if(disti==0){
      if(distj>0){
        diferencia = (80-fin.j)+ posicio.j;
        if(diferencia > distj){
          if(not enemic)executa(2,id,posicio,pio,false);
          else executa(6,id,posicio,pio,false);
        }
        else{
          if(not enemic)executa(6,id,posicio,pio,false);
          else return executa(2,id,posicio,pio,false);
        }
      }
      else {
        diferencia = (80-posicio.j)+fin.j;
        distj=distj*(-1);
        if(diferencia > distj){
          if(not enemic)executa(6,id,posicio,pio,false);
          else executa(2,id,posicio,pio,false);
        }
        else{
          if(not enemic)executa(2,id,posicio,pio,false);
          else executa(6,id,posicio,pio,false);
        }
      }
    }
    else if(distj==0){
      if(disti<0){
        if(not enemic)executa(4,id,posicio,pio,false);
        else executa(0,id,posicio,pio,false);
      }
      else{
        if(not enemic)executa(0,id,posicio,pio,false);
        else executa(4,id,posicio,pio,false);
      }
    }
    else{
      char movi, movj;
      if(disti>0){
        if(not enemic)movi = 'B';
        else movi = 'T';
      }
      else{
        if(not enemic)movi = 'T';
        else movi = 'B';
      }
      if(distj>0){
        diferencia = (80-fin.j)+ posicio.j;
        if(diferencia > distj){
          if(not enemic)movj = 'R';
          else movj = 'L';
        }
        else{
          if(not enemic)movj = 'L';
          else movj = 'R';
        }
      }
      else{
        diferencia = (80-posicio.j)+fin.j;
        distj=distj*(-1);
        if(diferencia > distj){
            if(not enemic)movj = 'L';
            else movj = 'R';
        }
        else{
          if(not enemic)movj = 'R';
          else movj = 'L';
        }
      }
      if(movi == 'T'){
        if(movj == 'R')executa(3,id,posicio,pio,false);
        else executa(5,id,posicio,pio,false);
      }
      else{
        if(movj == 'R')executa(1,id,posicio,pio,false);
        else executa(7,id,posicio,pio,false);
      }
    }

}

void BFSenemic(Pos pos, Unit meva, int id){
  set<Pos> visit;
  queue<Pos> P;
  P.push(pos);
  visit.insert(pos);
  bool trobat = false;
  bool contrari = false;
  Pos fin;
  while(not trobat and not P.empty()){
    Pos act = P.front();
    P.pop();
    Cell cel = cell(act);
    if(cel.type==Cave and cel.id!=-1){
      Unit enemic = unit(cel.id);
      if(enemic.player!=me()){
        if(enemic.type==Pioneer){
          trobat=true;
          fin=act;
        }
        else if(enemic.type==Furyan and meva.health >= enemic.health){
          trobat=true;
          fin=act;
        }
        else{
          trobat=true;
          contrari=true;
          fin=act;
        }
      }
    }
    for(int i=0; i<8; ++i){
      Pos seg = act+Dir(i);
      if(visit.find(seg)==visit.end()){
        if(pos_ok(seg) and not rock(seg))P.push(seg);
        visit.insert(seg);
      }
    }
  }
  if(contrari)escapa(pos,fin,id);
  else moua(pos,fin,contrari,id,false);
}

void BFScasella(Pos pos, int id){
  set<Pos> visit;
  queue<Pos> p;
  bool enemic=false;
  bool trobat = false;
  visit.insert(pos);
  p.push(pos);
  Pos fin;
  while(not trobat and not p.empty()){
    Pos act = p.front();
    p.pop();
    Cell cel = cell(act);
    if(cel.type==Cave and cel.id==-1 and cel.owner!=me()){
        trobat=true;
        fin=act;
    }
    else if(cel.type==Cave and cel.id!=-1){
        Unit u = unit(cel.id);
        if(u.type==Furyan and u.player!=me()){
            trobat=true;
            enemic=true;
            fin=act;
        }
        else if(u.type==Hellhound){
            trobat=true;
            enemic=true;
            fin=act;
        }
    }
    for(int i=0; i<8; ++i){
      Pos seg = act + Dir(i);
      if(visit.find(seg)==visit.end()){
          if(pos_ok(seg) and not rock(seg))p.push(seg);
          visit.insert(seg);
      }
    }
  }
  if(enemic)escapa(pos,fin,id);
  else moua(pos,fin,enemic,id,true);
}

bool BFShellhounds(Pos pos, int id,bool pio){
  bool fet=false;
  vector<int> H = hellhounds();
  int h = H.size();
  int i=0;
  while(not fet and i<h){
    Unit u = unit(H[i]);
    Pos fin = u.pos;
    if((abs(fin.i-pos.i)<3 and abs(fin.i-pos.i)>3)){
        escapa(pos,fin,id);
        fet=true;
    }
    ++i;
  }
  return fet;
}

void mouFuryans(){
  int m = me();
  vector<int> F = furyans(m);
  int f = F.size();
  bool pujar=false;
  for(int i=0; i<f; ++i){
    Unit u = unit(F[i]);
    Pos pos = u.pos;
    //executa(2,F[i],pos);
    //moua(pos,pos+Dir(random(0,7)),false,F[i],false);
    if(not BFShellhounds(pos,F[i],false))BFSenemic(pos,u,F[i]);
  }

}

void mouPioneers(){
  int m = me();
  vector<int> P = pioneers(m);
  int p = P.size();
  for(int i=0; i<p; ++i){
    Unit u = unit(P[i]);
    Pos pos = u.pos;
    //moua(pos,pos+Dir(Top),false,P[i],true);
    if(not BFShellhounds(pos,P[i],true))BFScasella(pos,P[i]);
  }
}

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    mouFuryans();
    mouPioneers();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
