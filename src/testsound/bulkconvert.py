#H78R Systems
#Converts all supported tracker files in the directory specified into .h files. 
#Calls 'xdd'. Didn't bother to reimplement this specifically. 
#Will include the original tracker extension in the filename for info. For example. An input file of example.s3m will output as example_s3m_data.h.


import os
import subprocess

SUPPORTED_EXTENSIONS = ['.mod', '.xm', '.s3m', '.it', '.stm']

def convert_tracker_files_to_h(directory):
    if not os.path.isdir(directory):
        print(f"Error: '{directory}' is not a valid directory.")
        return

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        name, ext = os.path.splitext(filename)

        if ext.lower() in SUPPORTED_EXTENSIONS:
            var_name = f"{name}{ext.replace('.', '_')}_data"
            output_filename = f"{var_name}.h"
            output_path = os.path.join(directory, output_filename)

            try:
                with open(output_path, 'w') as out_file:
                    subprocess.run(['xxd', '-i', '-n', var_name, file_path], stdout=out_file, check=True)
                print(f"Converted: {filename} -> {output_filename}")
            except subprocess.CalledProcessError as e:
                print(f"Failed to convert {filename}: {e}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python convert_tracker.py <directory>")
    else:
        convert_tracker_files_to_h(sys.argv[1])
