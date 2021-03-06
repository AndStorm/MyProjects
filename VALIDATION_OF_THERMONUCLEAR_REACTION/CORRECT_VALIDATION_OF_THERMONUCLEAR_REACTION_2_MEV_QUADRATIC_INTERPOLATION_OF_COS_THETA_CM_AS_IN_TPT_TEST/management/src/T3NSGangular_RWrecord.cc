#include "T3NSGangular_RWrecord.hh"

std::ofstream& T3NSGangular_RWrecord::save_binary(std::ofstream& out_stream) const
{
  if( out_stream.good() )
  {
    const unsigned int vector_size = size();
    out_stream.write((const char*) &levN, sizeof(T3int) );
    out_stream.write((const char*) &vector_size, sizeof(unsigned int) );
    for(T3NSGangular_RWrecord::const_iterator it = begin(); it != end(); ++it)
    {
      it->save_binary(out_stream);
    }
  }
  else T3cout<<"-Warning-T3NSGangular_RWrecord::save_binary:*Bad stream*"<<T3endl;
  return out_stream;
}

std::ifstream& T3NSGangular_RWrecord::load_binary(std::ifstream& in_stream )
{
  if( in_stream.good() )
  {
    unsigned int vector_size = 0;
    in_stream.read((char*) &levN, sizeof(T3int) );
    in_stream.read((char*) &vector_size, sizeof(unsigned int) );
    resize(vector_size);
    for(T3NSGangular_RWrecord::iterator it = begin(); it != end(); ++it)
    {
      it->load_binary(in_stream);
    }
  }
  else T3cout<<"-Warning-T3NSGangular_RWrecord::load_binary: *Bad stream*"<<T3endl;

//\\//***********************************//
  //std::cout<<*this<<std::endl;
//\\//***********************************//  
  
  return in_stream;
}

T3NSGangular_RWnode T3NSGangular_RWrecord::interpolate(T3double E) const
{
  T3NSGangular_RWnode res;
  if (!size())
  {
    T3cout << "T3NSGangular_RWrecord::interpolate: empty record" << T3endl;
    return res;
  }
  if( front().Get_E() > E)
  {
    T3cout << "T3NSGangular_RWrecord::interpolate: too high E=" << E << " > "
           << front().Get_E() << T3endl;
    return res;
  }
  // profile, maybe make logarithmic (std::lower_bound with less by E)
  // currently linear
  size_t ind =0;
  for(; ind + 1 < size(); ++ind) if( at(ind+1).Get_E() > E) break;
  if (ind + 1 >= size() ) return res;
  const T3double Elow  = at(ind    ).Get_E();
  const T3double Ehigh = at(ind + 1).Get_E();
  const T3double dE = Ehigh - Elow;
  const T3double sllow = at(ind    ).Get_sl();
  const T3double slhigh= at(ind + 1).Get_sl();
  const T3double prlow = at(ind    ).Get_pr();
  const T3double prhigh= at(ind + 1).Get_pr();
  // FIXME sum of exponents is not an exponent
  T3double sl;
  T3double pr;
  if (dE < 1e-10)
  {
    pr = (prlow + prhigh)/2;
    sl = (sllow + slhigh)/2;
  }
  else
  {
    pr = prlow + (prhigh - prlow)*(E - Elow)/dE;
    sl = sllow + (slhigh - sllow)*(E - Elow)/dE;
  }
  
  /////res.Set_E(E);
  
  res.Set_pr(pr);
  res.Set_sl(sl);
#ifdef debug
  T3cout << "E=" << E << " pr=" << pr << " sl=" << sl << T3endl;
#endif
  for(size_t i = 1; i <= T3NSGangular_RWnode::_num_point; ++i)
  {
    const T3double vlow  = at(ind    ).Get_V(i);
    const T3double vhigh = at(ind + 1).Get_V(i);
    if (vlow < -1 || 1 < vlow)
    { // TODO if pr > 1 - 1e-bigval fill non-exponent with isotropic
      // mybe if almost isotropic use exponent only
      T3cout << "T3NSGangular_RWrecord::interpolate: vlow=" << vlow << " i=" << i
             << " ind=" << ind << " Elow=" << Elow << " E=" << E << " Ehigh=" << Ehigh
             << " pr=" << pr << " sl=" << sl <<T3endl;
      T3cout << *this << T3endl;
    }
    if (vhigh < -1 || 1 < vhigh)
    {
      T3cout << "T3NSGangular_RWrecord::interpolate: vhigh=" << vhigh << " i=" << i
             << " ind=" << ind << " Elow=" << Elow << " E=" << E << " Ehigh=" << Ehigh
             << " pr=" << pr << " sl=" << sl <<T3endl;
      T3cout << *this << T3endl;
    }
    
    if (dE < 1e-10) res.Set_V(i, (vlow + vhigh)/2);
    else
    {
      const T3double val = vlow + (vhigh - vlow)*(E - Elow)/dE;
      if (val < -1 || 1 < val)
      {
        T3cout << "T3NSGangular_RWrecord::interpolate: val=" << val << " vlow=" << vlow
               << " vhigh=" << vhigh << " Elow=" << Elow << " Ehigh=" << Ehigh 
               << " E=" << E << T3endl;
      }
      res.Set_V(i, val );
    }
  }
  return res;
}

std::ostream& operator<<(std::ostream& os, const T3NSGangular_RWrecord& inst)
{
  os << "Record levN = " << inst.Get_levN() << ", nodes:";
  for(size_t ind =0; ind < inst.size(); ++ind) os << "\n\t" << inst.at(ind);
  return os;
}







