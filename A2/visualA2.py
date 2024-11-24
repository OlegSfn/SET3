import streamlit as st
import pandas as pd

def main():
    st.set_page_config(layout="wide")
    uploaded_files = st.file_uploader("Загрузите файл с данными", type=["txt", "csv"], accept_multiple_files=True)

    for file in uploaded_files:
        try:
            df = pd.read_csv(file, header=None)
            x_values = df.iloc[0].dropna().values
            range_arrays_values = df.iloc[1].dropna().values
            reversed_arrays_values = df.iloc[2].dropna().values
            almost_sorted_array_values = df.iloc[3].dropna().values

            if len(x_values) != len(range_arrays_values) or len(x_values) != len(reversed_arrays_values) or len(x_values) != len(almost_sorted_array_values):
                st.error("Ошибка: массивы должны быть одинаковой длины.")
                return
            
            data1 = pd.DataFrame({
                'x': x_values,
                'f(x)': range_arrays_values
            }).set_index('x')

            data2 = pd.DataFrame({
                'x': x_values,
                'f(x)': reversed_arrays_values
            }).set_index('x')
            
            data3 = pd.DataFrame({
                'x': x_values,
                'f(x)': almost_sorted_array_values
            }).set_index('x')

            st.write(file.name[:len(file.name)-4])
            columns = st.columns(3)
            with columns[0]:
                st.write('range arrays')
                st.line_chart(data1, x_label="Array size", y_label="Time")
            with columns[1]:
                st.write('reversed arrays')
                st.line_chart(data2, x_label="Array size", y_label="Time")
            with columns[2]:
                st.write('almost sorted arrays')
                st.line_chart(data3, x_label="Array size", y_label="Time")

        except Exception as e:
            st.error(f"Ошибка при обработке файла: {e}")

if __name__ == "__main__":
    main()