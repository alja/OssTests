#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"


#include "SimDataFormats/Associations/interface/LayerClusterToCaloParticleAssociatorBaseImpl.h"

void printClusterCaloParticleAssociation(fwlite::Event* event)
{
    /*
    typedef edm::AssociationMap<
        edm::OneToManyWithQualityGeneric<
            std::vector<CaloParticle>,
            std::vector<reco::CaloCluster>,
            std::pair<float, float>,
            unsigned int,
            edm::RefProd<vector<CaloParticle> >,
            edm::RefProd<vector<reco::CaloCluster> >,
            edm::Ref<vector<CaloParticle>,CaloParticle,edm::refhelper::FindUsingAdvance<vector<CaloParticle>,CaloParticle> >,
            edm::Ref<vector<reco::CaloCluster>,reco::CaloCluster,edm::refhelper::FindUsingAdvance<vector<reco::CaloCluster>,reco::CaloCluster> > 
            > 
            // OneToManyWithQualityGeneric
        > 
        // AssociationMap
        testTypeDef;
    */

    fwlite::Handle<hgcal::SimToRecoCollection> handle;
    handle.getByLabel(*event, "layerClusterCaloParticleAssociationProducer");

    printf("Associations CaloParticle to reco::CaloCluster valid=%d, ptr=%p, num_assocs=%d\n",
           handle.isValid(), handle.ptr(), (int)handle->size());

    int i = 0;
    for (auto ii = handle->begin(); ii != handle->end(); ++ii, ++i)
    {
        // auto helperKeyVal = *ii;
        auto &key = *ii->key; // key is edm::Ref -- so we de-ref it.
                              // std::cout << "type " << key.type;

        printf("  %d, index=%d something like energy=%f\n", i, ii->key.index(), key.energy());
        auto &val = ii->val; // presumably typedef std::vector<std::pair<ValRef, Q> > val_type
        int val_size = val.size();
        printf("   val vector size %d\n", val_size);

        for (int j = 0; j < val_size; ++j)
        {
            auto &ref_to_cc = *val[i].first;
            auto qq = val[i].second;
            printf("    %2d index=%u r=%f z=%f, ququs=%f, %f\n",
                   j, val[i].first.index(), ref_to_cc.position().r(), ref_to_cc.z(), qq.first, qq.second);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Need input file!" << std::endl;
        return 1;
    }
    auto file = TFile::Open(argv[1]);
    auto event_tree = dynamic_cast<TTree*>(file->Get("Events"));
    int nmax =  event_tree->GetEntries();
    printf("file has %d events \n", nmax);
    auto event = new fwlite::Event(file);


    for (int evtIdx = 0; evtIdx < nmax; ++evtIdx)
    {
        printf("event [%d]============================= \n", evtIdx);
        event->to(evtIdx);
        event_tree->LoadTree(evtIdx);
        printClusterCaloParticleAssociation(event);
        break; // all events are the same
    }
    return 0;
}