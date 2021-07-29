void plotDistortedSpectrum(const char * filename="am241_output.root"){


	// Load outputfile
	TFile * f = new TFile(filename, "READ");

	// Get histograms
	TH1D * rawspec = (TH1D*) f->Get("rawSpec");
	TH1D * distspec = (TH1D*) f->Get("corrSpec");



	TCanvas * c = new TCanvas();

	c->SetLogy();
	distspec->SetLineWidth(3);
	distspec->SetLineColor(kBlue);
	distspec->GetXaxis()->SetTitle("Energy (keV)");
	distspec->GetYaxis()->SetTitle("Counts (0.2 keV^{-1})");
	distspec->SetTitle("");
	distspec->Draw("hist");


	rawspec->SetLineColor(kBlack);
	rawspec->SetLineWidth(3);
	rawspec->Draw("histsame");

	TLegend * leg = new TLegend(0.5, 0.7, 0.9, 0.9);
	leg->AddEntry(rawspec, "Raw Spectrum");
	leg->AddEntry(distspec, "PCC Corrected Spectrum");
	leg->Draw();



}