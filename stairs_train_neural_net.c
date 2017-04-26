#include "fann.h"

int main(int argc, char **argv)
{
    const unsigned int num_input = 17;
    const unsigned int num_output = 3;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.0001;
    const unsigned int max_epochs = 10000;
    const unsigned int epochs_between_reports = 100;

    char *train_file_name = argv[1];

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output (ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, train_file_name, max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, "TEST.net");

    fann_destroy(ann);

    return 0;
}
