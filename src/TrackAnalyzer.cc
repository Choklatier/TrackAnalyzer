// -*- C++ -*-
//
// Package:    TrackAnalyzer
// Class:      TrackAnalyzer
// 
/**\class TrackAnalyzer TrackAnalyzer.cc LLP/TrackAnalyzer/src/TrackAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Mon Apr 27 23:55:07 BST 2026
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
//#include "FWCore/Framework/interface/EDGetToken.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
//
// class declaration
//

class TrackAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrackAnalyzer(const edm::ParameterSet&);
      ~TrackAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      // Tokens
      //edm::EDGetTokenT<reco::TrackCollection> trackToken_;
      //edm::EDGetTokenT<reco::VertexCollection> vertexToken_;
      //edm::EDGetTokenT<reco::PFMETCollection> metToken_;
      edm::InputTag trackTag_;
      edm::InputTag vertexTag_;
      edm::InputTag metTag_;

      // Tree
      TTree* tree_;

      // Event-level
      float met_pt;
      float met_phi;
      int nTrack;

      // Track variables
      std::vector<float> trk_pt;
      std::vector<float> trk_eta;
      std::vector<float> trk_phi;
      std::vector<float> trk_d0;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TrackAnalyzer::TrackAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   // trackToken_  = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("tracks"));
   // vertexToken_ = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"));
   // metToken_    = consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("met"));
   trackTag_ = iConfig.getParameter<edm::InputTag>("tracks");
   vertexTag_ = iConfig.getParameter<edm::InputTag>("vertices");
   metTag_ =   iConfig.getParameter<edm::InputTag>("met");

    edm::Service<TFileService> fs;

    tree_ = fs->make<TTree>("Events", "Events");

    tree_->Branch("met_pt", &met_pt, "met_pt/F");
    tree_->Branch("met_phi", &met_phi, "met_phi/F");
    tree_->Branch("nTrack", &nTrack, "nTrack/I");

    tree_->Branch("trk_pt", &trk_pt);
    tree_->Branch("trk_eta", &trk_eta);
    tree_->Branch("trk_phi", &trk_phi);
    tree_->Branch("trk_d0", &trk_d0);

}


TrackAnalyzer::~TrackAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



   //#ifdef THIS_IS_AN_EVENT_EXAMPLE
   //Handle<ExampleData> pIn;
   //iEvent.getByLabel("example",pIn);
   //#endif
   
     //#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
     //ESHandle<SetupData> pSetup;
   //iSetup.get<SetupRecord>().get(pSetup);
   //#endif

  // Clear vectors
  trk_pt.clear();
  trk_eta.clear();
  trk_phi.clear();
  trk_d0.clear();

  // Get collections
  edm::Handle<reco::TrackCollection> tracks;
  //iEvent.getByToken(trackToken_, tracks);
  iEvent.getByLabel(trackTag_, tracks);

  edm::Handle<std::vector<reco::Vertex> > vertices;
  //iEvent.getByToken(vertexToken_, vertices);
  iEvent.getByLabel(vertexTag_, vertices);

  //edm::Handle<reco::PFMETCollection> mets;
  //iEvent.getByToken(metToken_, mets);
  edm::Handle<std::vector<reco::PFMET> > mets;
  iEvent.getByLabel(metTag_, mets);
  
  if (!tracks.isValid()) return;
  if (!vertices.isValid()) return;
  if (!mets.isValid()) return;


  // MET
  if (!mets->empty()) {
    met_pt  = mets->front().pt();
    met_phi = mets->front().phi();
  } else {
    met_pt = -1;
    met_phi = 0;
  }

  // Primary vertex
  if (vertices->empty()) return;
  const reco::Vertex& pv = vertices->front();

  nTrack = 0;

  //for (const auto& trk : *tracks) {
  for (size_t i = 0; i<tracks->size();i++) {
    const reco::Track& trk = tracks->at(i); 
    if (!trk.quality(reco::TrackBase::highPurity)) continue;
    if (trk.pt() < 1.0) continue;

    trk_pt.push_back(trk.pt());
    trk_eta.push_back(trk.eta());
    trk_phi.push_back(trk.phi());

    float d0 = trk.dxy(pv.position());
    trk_d0.push_back(d0);

    nTrack++;
  }

  tree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
TrackAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TrackAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TrackAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TrackAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackAnalyzer);
