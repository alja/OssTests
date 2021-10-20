#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"

#include "FWCore/Framework/interface/GenericHandle.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "Fireworks/Core/src/FWGenericHandle.h"


#include "TClass.h"
// #include "Cintex/Cintex.h"

//========================================================================

struct App
{
   TFile              *m_file;
   TTree              *m_event_tree;
   fwlite::Event      *m_event;

   //------------------------------------------------------

   App(const char* fname)
   {
      m_file = TFile::Open(fname);
      m_event_tree = dynamic_cast<TTree*>(m_file->Get("Events"));
      m_event = 0;
      try
      {
         m_event = new fwlite::Event(m_file);
      }
      catch (const cms::Exception& iE)
      {
         printf("can't create a fwlite::Event\n");
         std::cerr << iE.what() <<std::endl;
         throw;
      }
   }

   ~App()
   {
      delete m_event;
      delete m_file;
   }

   //------------------------------------------------------

   void goto_event(Long64_t tid)
   {
      m_event->to(tid);
      m_event_tree->LoadTree(tid);
   }

   //------------------------------------------------------

   const reco::BeamSpot* checkBeamSpot()
   { 
      try
      {
         edm::InputTag tag("offlineBeamSpot");
         edm::Handle<reco::BeamSpot> spot;

         m_event->getByLabel(tag, spot);
         if (spot.isValid())
         {
            return spot.product();
         }
      }
      catch (cms::Exception& iException)
      {
         std::cerr <<"Can't get beam spot info. Setting coordintes to (0, 0, 0).\n";
      }

      return 0;
   }

   //------------------------------------------------------

   void checkPFCandidatesSimple()
   {
      printf("checkPFCandidatesSimple ....\n");
      try
      {
         edm::InputTag tag("particleFlow", "", "RECO");
         edm::Handle<std::vector<reco::PFCandidate>>  handle;

         m_event->getByLabel(tag, handle);
         if (handle.isValid())
         {
            printf("Simple valid PFCandidate  handle \n");
            return;
         }
         else
         {
            printf("Invalid PFCandidate handle !!\n");
         }
      }
      catch (cms::Exception &iException)
      {
         std::cerr << "Can't get PFCandidate info.\n";
      }
   }
   //------------------------------------------------------

   void checkPFCandidatesFW()
   { 
      printf("checkPFCandidatesFW ....\n");
      try
      {
         edm::InputTag tag("particleFlow", "", "RECO");
         TClass* ct = TClass::GetClass("std::vector<reco::PFCandidate>");
         edm::TypeWithDict type(*(ct->GetTypeInfo()));
         edm::FWGenericHandle handle(type);
         // XXX ???? edm::Handle<std::vector<reco::PFCandidate> > can;

         m_event->getByLabel(tag, handle);
         if (handle.isValid())
         {
            printf("valid PFCandidate  handle \n");
            return;
         }
         else { printf("Invalid PFCandidate handle !!\n");}
      }
      catch (cms::Exception& iException)
      {
         std::cerr <<"Can't get PFCandidate info.\n";
      }
   }
};

//========================================================================

int main(int argc, char* argv[])
{
   if (argc < 2)
   {
      std::cout << "Need input file!" << std::endl;
      return 1;
   }

   //ROOT::Cintex::Cintex::Enable();

   App app(argv[1]);

   app.goto_event(0);
   app.checkPFCandidatesSimple();
 
   return 0;
}
