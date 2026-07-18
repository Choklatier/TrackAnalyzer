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
// #include "FWCore/Framework/interface/EDGetToken.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TMath.h"
//
// class declaration
//

class TrackAnalyzer : public edm::EDAnalyzer
{
public:
  explicit TrackAnalyzer(const edm::ParameterSet &);
  ~TrackAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob();
  virtual void analyze(const edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  virtual void beginRun(edm::Run const &, edm::EventSetup const &);
  virtual void endRun(edm::Run const &, edm::EventSetup const &);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &);
  virtual void endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &);

  // ----------member data ---------------------------
  // Tokens
  // edm::EDGetTokenT<reco::TrackCollection> trackToken_;
  // edm::EDGetTokenT<reco::VertexCollection> vertexToken_;
  // edm::EDGetTokenT<reco::PFMETCollection> metToken_;
  edm::InputTag trackTag_;
  edm::InputTag vertexTag_;
  edm::InputTag metTag_;
  edm::InputTag jetTag_;

  // Tree
  TTree *tree_;

  // Event-level
  float met_pt;
  float met_phi;
  int nTrack;
  int nJet;
  int nVertex;
  float H_T;

  // Track variables
  std::vector<float> trk_pt;
  std::vector<float> trk_eta;
  std::vector<float> trk_phi;
  std::vector<float> trk_px;
  std::vector<float> trk_py;
  std::vector<float> trk_pz;
  std::vector<float> trk_charge;
  std::vector<float> trk_z0;
  std::vector<float> trk_d0;
  std::vector<int> trk_jetIdx;

  // Jet variables
  std::vector<float> jet_pt;
  std::vector<float> jet_eta;
  std::vector<float> jet_phi;
  std::vector<float> jet_mass;
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
TrackAnalyzer::TrackAnalyzer(const edm::ParameterSet &iConfig)

{
  // now do what ever initialization is needed
  //  trackToken_  = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("tracks"));
  //  vertexToken_ = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"));
  //  metToken_    = consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("met"));
  trackTag_ = iConfig.getParameter<edm::InputTag>("tracks");
  vertexTag_ = iConfig.getParameter<edm::InputTag>("vertices");
  metTag_ = iConfig.getParameter<edm::InputTag>("met");
  jetTag_ = iConfig.getParameter<edm::InputTag>("jets");

  edm::Service<TFileService> fs;

  tree_ = fs->make<TTree>("Events", "Events");

  tree_->Branch("met_pt", &met_pt, "met_pt/F");
  tree_->Branch("met_phi", &met_phi, "met_phi/F");
  tree_->Branch("nTrack", &nTrack, "nTrack/I");
  tree_->Branch("nJet", &nJet, "nJet/I");
  tree_->Branch("nVertex", &nVertex, "nVertex/I");
  tree_->Branch("H_T", &H_T, "H_T/F");

  tree_->Branch("trk_pt", &trk_pt);
  tree_->Branch("trk_eta", &trk_eta);
  tree_->Branch("trk_phi", &trk_phi);
  tree_->Branch("trk_px", &trk_px);
  tree_->Branch("trk_py", &trk_py);
  tree_->Branch("trk_pz", &trk_pz);
  tree_->Branch("trk_charge", &trk_charge);
  tree_->Branch("trk_z0", &trk_z0);
  tree_->Branch("trk_d0", &trk_d0);
  tree_->Branch("trk_jetIdx", &trk_jetIdx);

  tree_->Branch("jet_pt", &jet_pt);
  tree_->Branch("jet_eta", &jet_eta);
  tree_->Branch("jet_phi", &jet_phi);
  tree_->Branch("jet_mass", &jet_mass);
}

TrackAnalyzer::~TrackAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// Struct to define iterator to sort tracks by pT
struct TrackPtSorter
{
  bool operator()(const reco::Track* a, const reco::Track* b) const
  {
    return a->pt() > b->pt(); // descending pT
  }
};

// ------------ method called for each event  ------------
void TrackAnalyzer::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  using namespace edm;

  // #ifdef THIS_IS_AN_EVENT_EXAMPLE
  // Handle<ExampleData> pIn;
  // iEvent.getByLabel("example",pIn);
  // #endif

  // #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  // ESHandle<SetupData> pSetup;
  // iSetup.get<SetupRecord>().get(pSetup);
  // #endif

  // Clear vectors
  trk_pt.clear();
  trk_eta.clear();
  trk_phi.clear();
  trk_px.clear();
  trk_py.clear();
  trk_pz.clear();
  trk_charge.clear();
  trk_z0.clear();
  trk_d0.clear();
  trk_jetIdx.clear();
  jet_pt.clear();
  jet_eta.clear();
  jet_phi.clear();
  jet_mass.clear();

  // reset event level observables
  met_pt = -99;
  met_phi = -99;
  nTrack = -99;
  nJet = -99;
  nVertex = -99;
  H_T = -99;

  // Get collections
  edm::Handle<reco::TrackCollection> tracks;
  // iEvent.getByToken(trackToken_, tracks);
  iEvent.getByLabel(trackTag_, tracks);

  edm::Handle<std::vector<reco::Vertex> > vertices;
  // iEvent.getByToken(vertexToken_, vertices);
  iEvent.getByLabel(vertexTag_, vertices);

  // edm::Handle<reco::PFMETCollection> mets;
  // iEvent.getByToken(metToken_, mets);
  edm::Handle<std::vector<reco::PFMET> > mets;
  iEvent.getByLabel(metTag_, mets);

  edm::Handle<reco::PFJetCollection> jets;
  iEvent.getByLabel(jetTag_, jets);

  // std::cout << "jets valid: " << jets.isValid()
  //         << " size: " << (jets.isValid() ? jets->size() : -1)
  //         << std::endl;

  if (!tracks.isValid())
    return;
  if (!vertices.isValid())
    return;
  if (!mets.isValid())
    return;
  if (!jets.isValid())
    return;

  // MET
  if (!mets->empty())
  {
    met_pt = mets->front().pt();
    met_phi = mets->front().phi();
  }
  else
  {
    met_pt = -1;
    met_phi = -99.;
  }

  // Primary vertex
  if (vertices->empty())
    return;
  const reco::Vertex &pv = vertices->front();

  nVertex = vertices->size();

  // Fill jet information
  nJet = 0;
  H_T = 0;
  for (size_t j = 0; j < jets->size(); j++)
  {
    const reco::PFJet &jet = jets->at(j);
    jet_pt.push_back(jet.pt());
    jet_eta.push_back(jet.eta());
    jet_phi.push_back(jet.phi());
    jet_mass.push_back(jet.mass());
    nJet++;
    H_T += jet.pt();
  }

  nTrack = 0;
  
  // Sort tracks per pT
  std::vector<const reco::Track*> sortedTracks;
  sortedTracks.reserve(tracks->size());

 reco::TrackCollection::const_iterator it;
  for (it = tracks->begin(); it != tracks->end(); ++it)
  {
    sortedTracks.push_back(&(*it));
  }
  
  std::sort(sortedTracks.begin(), sortedTracks.end(), TrackPtSorter());


  // for (const auto& trk : *tracks) {
  for (size_t i = 0; i < tracks->size(); i++)
  {
    // const reco::Track &trk = tracks->at(i);
    const reco::Track &trk = *sortedTracks[i]; //tracks->at(i);

    // Require high purity for the tracks
    if (!trk.quality(reco::TrackBase::highPurity)) 
      continue;
    
    // require a minimum number of valid hits
    if (trk.numberOfValidHits() < 6){
      continue;
    }

    // require decent chi2
    if (trk.normalizedChi2() >= 5){
      continue;
    }

    // Minimum pT cut
    if (trk.pt() < 1.0) // Stop when tracks at lower than 1 GeV
      break;

    trk_pt.push_back(trk.pt());
    trk_eta.push_back(trk.eta());
    trk_phi.push_back(trk.phi());
    trk_px.push_back(trk.px());
    trk_py.push_back(trk.py());
    trk_pz.push_back(trk.pz());
    trk_charge.push_back(trk.charge());

    float d0 = trk.dxy(pv.position());
    float z0 = trk.dz(pv.position());
    trk_d0.push_back(d0);
    trk_z0.push_back(z0);

    // Match track to nearest jet
    int matchedJetIdx = -1;
    float jetRadius = 0.5;
    for (size_t j = 0; j < jets->size(); j++)
    {
      const reco::PFJet &jet = jets->at(j);
      float dEta = trk.eta() - jet.eta();
      float dPhi = trk.phi() - jet.phi();
      // Normalize dPhi to [-pi, pi]
      while (dPhi > M_PI)
        dPhi -= 2 * M_PI;
      while (dPhi < -M_PI)
        dPhi += 2 * M_PI;
      float dR = TMath::Sqrt(dEta * dEta + dPhi * dPhi);
      if (dR < jetRadius)
      {
        matchedJetIdx = (int)j;
        break; // Match to the first jet within radius
      }
    }
    trk_jetIdx.push_back(matchedJetIdx);

    nTrack++;
  }

  // Ask for a minimum amount of tracks for the events!
  if (nTrack < 5){
    return;
  }

  tree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void TrackAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void TrackAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
void TrackAnalyzer::beginRun(edm::Run const &, edm::EventSetup const &)
{
}

// ------------ method called when ending the processing of a run  ------------
void TrackAnalyzer::endRun(edm::Run const &, edm::EventSetup const &)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void TrackAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void TrackAnalyzer::endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TrackAnalyzer::fillDescriptions(edm::ConfigurationDescriptions &descriptions)
{
  // The following says we do not know what parameters are allowed so do no validation
  //  Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// define this as a plug-in
DEFINE_FWK_MODULE(TrackAnalyzer);
