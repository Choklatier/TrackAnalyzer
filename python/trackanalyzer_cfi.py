import FWCore.ParameterSet.Config as cms

process = cms.Process("LLP")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #"root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Mu/AOD/Apr21ReReco-v1/0000/00459D48-EB70-E011-AF09-90E6BA19A252.root"
        # "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/MultiJet/AOD/Apr21ReReco-v1/0000/002A8227-7471-E011-9632-002618943838.root"
        # "root://eospublic.cern.ch//eos/opendata/cms/Run2010A/MinimumBias/AOD/Apr21ReReco-v1/0000/0030CAFF-0B6D-E011-8202-0025902008DC.root"

        *list(open("datasets/CMS_Run2010B_MultiJet_AOD_Apr21ReReco-v1_0000_file_index.txt","r").read().splitlines())
    )   
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("output.root")
)


process.analyzer = cms.EDAnalyzer('TrackAnalyzer',
                                  tracks = cms.InputTag("generalTracks"),
                                  vertices = cms.InputTag("offlinePrimaryVertices"),
                                  met = cms.InputTag("pfMet"),
                                  jets = cms.InputTag("ak5PFJets"),
                                  )

process.p = cms.Path(process.analyzer)
