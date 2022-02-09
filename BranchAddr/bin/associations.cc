#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"

#include "SimDataFormats/Associations/interface/LayerClusterToCaloParticleAssociatorBaseImpl.h"
#include "SimDataFormats/Associations/interface/LayerClusterToSimClusterAssociatorBaseImpl.h"

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
        for (int j = 0; j < val_size; ++j)
        {
            auto &ref_to_cc = *val[j].first;
            auto qq = val[j].second;
            printf("reco::CaloCluster   %2d index=%2u r=%f z=%f, quality=%f, %f\n",
                   j, val[j].first.index(), ref_to_cc.position().r(), ref_to_cc.z(), qq.first, qq.second);
        }
    }
}
//------------------------------------------------------------------------------------------------
void printCaloParticleClusterAssociation(fwlite::Event* event)
{
    /*
edm::AssociationMap<edm::OneToManyWithQualityGeneric<
vector<reco::CaloCluster>,
vector<CaloParticle>,float,
unsigned int,
edm::RefProd<vector<reco::CaloCluster> >,
edm::RefProd<vector<CaloParticle> >,
edm::Ref<vector<reco::CaloCluster>,reco::CaloCluster,edm::refhelper::FindUsingAdvance<vector<reco::CaloCluster>,reco::CaloCluster> >,
edm::Ref<vector<CaloParticle>,CaloParticle,edm::refhelper::FindUsingAdvance<vector<CaloParticle>,CaloParticle> > > >
*/

    fwlite::Handle<hgcal::RecoToSimCollection> handle;
    handle.getByLabel(*event, "layerClusterCaloParticleAssociationProducer");

    printf("\nAssociations reco::ClusterCluster to CaloParticle  num_associations = %lu\n", handle->size());

    for (auto ii = handle->begin(); ii != handle->end(); ++ii)
    {
        auto &val = ii->val; // presumably typedef std::vector<std::pair<ValRef, Q> > val_type
        printf("reco::CaloCluster idx = %d associated with %lu CaloParicles:\n", ii->key.index(), val.size());
        for (unsigned int j = 0; j < val.size(); ++j)
        {
            auto quality = val[j].second;
            printf("CaloParticle ref idx = [%2u] qualitity=(%f)\n", val[j].first.index(), quality);
        }
        printf("\n");
    }
}

//------------------------------------------------------------------------------------------------
void printClusterSimClusterAssociation(fwlite::Event* event)
{
    /*
    edm::AssociationMap<
    edm::OneToManyWithQualityGeneric<
    vector<SimCluster>,
    vector<reco::CaloCluster>,
    pair<float,float>,
    unsigned int,
    edm::RefProd<vector<SimCluster> >,
    edm::RefProd<vector<reco::CaloCluster> >,
    edm::Ref<vector<SimCluster>,SimCluster,edm::refhelper::FindUsingAdvance<vector<SimCluster>,SimCluster> >,
    edm::Ref<vector<reco::CaloCluster>,reco::CaloCluster,edm::refhelper::FindUsingAdvance<vector<reco::CaloCluster>,reco::CaloCluster> > 
     > 
     >
*/
    fwlite::Handle<hgcal::SimToRecoCollectionWithSimClusters> handle;
    handle.getByLabel(*event, "layerClusterSimClusterAssociationProducer");
    printf("\nAssociations SimCluster to reco::CaloClusters  num_association=%lu \n", handle->size());

    for (auto ii = handle->begin(); ii != handle->end(); ++ii)
    {
        printf("SimCluster idx = %d associated with cluster indices:\n", ii->key.index());
        auto &val = ii->val; // presumably typedef std::vector<std::pair<ValRef, Q> > val_type
        for (unsigned int j = 0; j < val.size(); ++j)
        {
            auto quality = val[j].second;
            printf("reco::CaloCluster ref idx = %2u qualitites=(%f, %f)\n", val[j].first.index(), quality.first, quality.second);
        }
        printf("\n");
    }
}
//------------------------------------------------------------------------------------------------
void printSimClusterClusterAssociation(fwlite::Event* event)
{ 
    /* OneToManyWithQualityGeneric<vector<reco::CaloCluster>,
    vector<SimCluster>,
    float,unsigned int,
    edm::RefProd<vector<reco::CaloCluster> >,
    edm::RefProd<vector<SimCluster> >,
    edm::Ref<vector<reco::CaloCluster>,reco::CaloCluster,edm::refhelper::FindUsingAdvance<vector<reco::CaloCluster>,reco::CaloCluster> >,
    edm::Ref<vector<SimCluster>,SimCluster,edm::refhelper::FindUsingAdvance<vector<SimCluster>,SimCluster> > > >  
    */
    fwlite::Handle<hgcal::RecoToSimCollectionWithSimClusters> handle;
    handle.getByLabel(*event, "layerClusterSimClusterAssociationProducer");
    printf("\nAssociations reco::CaloClusters to SimCluster num_association=%lu \n", handle->size());

    for (auto ii = handle->begin(); ii != handle->end(); ++ii)
    {
        printf("reco::CaloCluster idx = %d associated with cluster indices:\n", ii->key.index());
        auto &val = ii->val; // presumably typedef std::vector<std::pair<ValRef, Q> > val_type
        for (unsigned int j = 0; j < val.size(); ++j)
        {
            auto quality = val[j].second;
            printf("SimCluster ref idx = %2u quality=(%f)\n", val[j].first.index(), quality);
        }
        printf("\n");
    }
}

//------------------------------------------------------------------------------------------------
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


    for (int e = 0; e < nmax; e++)
    {
        printf("event [%d]============================= \n", e);
        event->to(e);
        event_tree->LoadTree(e);

        printClusterCaloParticleAssociation(event);
        printCaloParticleClusterAssociation(event);
        
        //printClusterSimClusterAssociation(event);
        //printSimClusterClusterAssociation(event);
        break; // at the moment check only first event
    }
    return 0;
}
