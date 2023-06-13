#
# Example python script to generate a BOM from a KiCad generic netlist
#
# Example: Sorted and Grouped CSV BOM
#

"""
    @package
    Generate a CSV BOM for use with JLCSMT service.
    Components are sorted by ref and grouped by value with same footprint
    Fields are (if exist).
    LCSC Part numbers are copied from the "LCSC" field, if exists.
    

    Output fields:
    'Comment', 'Designator', 'Footprint', 'LCSC Part Number'

    Command line:
    python "pathToFile/bom_csv_jlcsmt.py" "%I" "%O-JLC.csv"
"""

import kicad_netlist_reader
import csv
import sys

net = kicad_netlist_reader.netlist(sys.argv[1])

# A helper function to convert a UTF8/Unicode/locale string read in netlist
# for python2 or python3
def fromNetlistText( aText ):
    if sys.platform.startswith('win32'):
        try:
            return aText.encode('utf-8').decode('cp1252')
        except UnicodeDecodeError:
            return aText
    else:
        return aText

with open(sys.argv[2], 'w') as f:
    out = csv.writer(f)
    out.writerow(['Comment', 'Designator', 'Footprint', 'LCSC Part Number'])

    for group in net.groupComponents():
        refs = ""
        lcsc_pn = ""
        # Add the reference of every component in the group and keep a reference
        # to the component so that the other data can be filled in once per group
        for component in group:
            refs += fromNetlistText( component.getRef() ) + ", "
            lcsc_pn = component.getField("LCSC") or lcsc_pn
            c = component
        
        if lcsc_pn == "":
            continue
        
        out.writerow([
            fromNetlistText(c.getValue()) + " " + fromNetlistText(c.getDescription()),
            refs,
            fromNetlistText( c.getFootprint() ),
            fromNetlistText( lcsc_pn )
        ]) 

    f.close()
