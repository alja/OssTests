#ifndef FireworksWeb_Calo_scaledMarker_h
#define FireworksWeb_Calo_scaledMarker_h

namespace ROOT {
namespace Experimental {
class REveScalableStraightLineSet;
//class FWViewContext;
}
}

namespace fireworks
{
struct scaleMarker {
   scaleMarker(ROOT::Experimental::REveScalableStraightLineSet* ls, float et, float e):
      m_ls(ls),
      m_et(et),
      m_energy(e)
   {
   };

   virtual ~scaleMarker() {}

   ROOT::Experimental::REveScalableStraightLineSet* m_ls;
   float m_et;
   float m_energy;
   // const FWViewContext* m_vc;
};
}

#endif
