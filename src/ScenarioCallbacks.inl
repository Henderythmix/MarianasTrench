int ResearchShark() {
    Textbox("After analasis of behaviour, you realize they are not necessarily dangerous to humans.");
    Sub.Discover(50);
    return 0;
}

int PetShark() {
    Textbox("I have no idea how you managed to do that, but you made the shark happy.");
    Sub.Discover(500);
    Sub.DrainOxygen(-50);
    return 0;
}

int ResearchAggresiveShark() {
    Textbox("The shark bumps and scratches the sub with its fins. The shell of your submarine is damaged.");
    Sub.TakeDamage(Sub.getResistance()/2);
    Sub.DrainOxygen(Sub.getOxygen()/2);

    return 0;
}

int ResearchWhale() {
    static bool SrsOrNo{true};
    SrsOrNo = !SrsOrNo;

    if (SrsOrNo) {
        Textbox("The whale does not look hungry. It makes you wonder how often it eats.");
        Sub.Discover(100);
        return 0;
    }
    Textbox("Big whale :D");
    Sub.Discover(10);
    return 0;
}

int ResearchOctopus() {
    Textbox("The hypothesis of the head being bigger than a squid can be proven by the analysis you have recorded.");
    Sub.Discover(5);
    return 0;
}

int ResearchSquid() {
    Textbox("The hypothesis of the head being smaller than an octopus can be confirmed by the analisis.");
    Sub.Discover(5);
    return 0;
}

int WaitFish() {
    Textbox("You politely wait for the fish to pass. you observe each single fish to record into your notes,");
    Sub.DrainOxygen(2);
    Sub.Discover(100);

    return 0;
}

int ScareFish() {
    Textbox("You scared the fish by continuing to drive through :(");

    return 0;
}

int Continue() {
    Textbox("");
    return 0;
}