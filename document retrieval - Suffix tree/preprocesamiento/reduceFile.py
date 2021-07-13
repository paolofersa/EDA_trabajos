import json

def write_lines_to_json(input_lines, indent="  "):
        yield "["

        first_line = True
        for line in input_lines:
            data = ""
            if first_line:
                first_line = False
            else:
                data += ","

            data += "\n" + indent + line.strip()
            yield data

        yield "\n]"

with open("fuente.json") as infile, open("output.json", "w") as outfile:
        for data in write_lines_to_json(infile):
            outfile.write(data)