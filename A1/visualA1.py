import streamlit as st
import pandas as pd
import os

def main():
    st.set_page_config(layout="wide")

    file_directory = '.'
    filetype = '.txt'
    txt_files = [f for f in os.listdir(file_directory) if f.endswith(filetype)]

    if txt_files:
        for filename in txt_files:
            try:
                # Construct the full file path
                file_path = os.path.join(file_directory, filename)
                df = pd.read_csv(file_path, header=None)
                x_values = df.iloc[0].dropna().values
                fx_values = df.iloc[1].dropna().values
                diffs_values = df.iloc[2].dropna().values

                if len(x_values) != len(fx_values):
                    st.error("Ошибка: массивы x и f(x) должны быть одинаковой длины.")
                    return
                
                data1 = pd.DataFrame({
                    'x': x_values,
                    'f(x)': fx_values
                }).set_index('x')

                data2 = pd.DataFrame({
                    'x': x_values,
                    'f(x)': diffs_values
                }).set_index('x')

                st.write(file_path.rstrip('.txt')[2:])
                columns = st.columns(2)
                with columns[0]:
                    st.write('Absolute area')
                    st.line_chart(data1, x_label="Points count", y_label="S")
                with columns[1]:
                    st.write('Relative area')
                    st.line_chart(data2, x_label="Points count", y_label="Relative accuracy")

            except Exception as e:
                st.error(f"Ошибка при обработке файла: {e}")

if __name__ == "__main__":
    main()