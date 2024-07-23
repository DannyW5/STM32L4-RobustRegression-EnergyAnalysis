import pandas as pd
import os

class Csv2CppHeaderConverter:

    HPP_TEMPLATE = """\
    #ifndef {header_guard}
    #define {header_guard}

    #include <array>
    #include "algorithms/Point.hpp"

    const std::array<Point<double>, {size}> points = {{{{
    {points}
    }}}};

    #endif // {header_guard}
    """

    @staticmethod
    def convert_data(filepath: str):
        df = pd.read_csv(filepath)
        points= ",\n".join([f"    {{{row['X']}, {row['Y']}}}" for _, row in df.iterrows()])
        header_guard = f"{(filepath.split('/')[-1])[:-4].upper()}_HPP"
        hpp_content = Csv2CppHeaderConverter.HPP_TEMPLATE.format(size=len(df), points=points, header_guard=header_guard)

        with open(f"STM32_Source_Code/inc/data/{filepath.split('/')[-1][:-4]}.hpp", 'w') as f:
            f.write(hpp_content)

if __name__ == "__main__":
    Csv2CppHeaderConverter.convert_data("Python_Data_Generation/data/DataLength1000.csv")
