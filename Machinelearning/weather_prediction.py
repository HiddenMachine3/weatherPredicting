
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import  LinearRegression
from sklearn.model_selection import train_test_split
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import  LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
from sklearn.ensemble import RandomForestRegressor
from xgboost import XGBRegressor
import streamlit as st
# st.markdown(
#     """
#     <style>
#     .sidebar .sidebar-content {
#         background-color: #001f3f;
#         color: white;
#     }
#     </style>
#     """,
#     unsafe_allow_html=True
# )

# # Add content to the sidebar
# st.sidebar.header("")
st.title("Weather Prediction and Analysis")
st.write("")
st.write("\n")
df=pd.read_excel("Weather_test.xlsx")
df1=pd.read_csv("Weather_data.csv")
st.write("<p style='font-size: 34px;'>Dataframe used</p>",unsafe_allow_html=True)
st.dataframe(df)
df.drop("Timestamp",inplace=True,axis=1)
df.set_index("ID")
df["Date"]=pd.to_datetime(df["Date"])
df["Date"]=pd.to_datetime(df["Date"])
X=df[["Day","Month","Hour"]]
Y=df[["Temperature","Humidity"]]

X_train,X_test,Y_train,Y_test=train_test_split(X,Y,test_size=0.2,random_state=42)

HourInput=st.text_input("Enter the Hour")
st.write("Hour is ",HourInput)
DayInput=st.text_input("Enter the day")
st.write("Day is ",DayInput)
MonthInput=st.text_input("Enter the month")
st.write("Month is ",MonthInput)
Year_Input=st.text_input("Enter the Year")
st.write("Year is ",Year_Input)
Input=pd.DataFrame({"Day":[DayInput],"Month":[MonthInput],"Hour":[HourInput]})
st.write("<p style='font-size: 34px;'>Temperature prediction for the hour {} of the day {} of month {} of the year {}</p>".format(HourInput, DayInput, MonthInput, Year_Input), unsafe_allow_html=True)
model=LinearRegression()
model.fit(X_train,Y_train)
Y_pred=model.predict(X_test)
mean_error=mean_squared_error(Y_test,Y_pred)
score=model.score(X_test,Y_test)
print(score)
print("Mean squared error is ",mean_error)
predict=model.predict(Input)
print(f"The temperature and humidity of the day {DayInput} of {MonthInput} and {HourInput} is ")
print("Temp Predict: ",predict[0][0])
print("Humidity predict: ",predict[0][1])
# st.write("Linear Regression")
# st.write(model.coef_)
# st.write(model.intercept_)
# x=X_train
# x=np.linspace(0,100,100)
# y=model.predict(x.shape(-1,-1))   
# plt.plot(X_train,Y_train)
# st.pyplot(plt)
plt.figure(figsize=(19,29))
graph=plt.plot(X_train["Hour"],Y_train["Temperature"])
plt.show()
st.write("\n")
st.write("<p style='font-size: 34px;'>Linear Regression</p>",unsafe_allow_html=True)
st.write("Accuracy is : ",score*100)
st.write("Temperature predicted is : ",round(predict[0][0],2))
st.write("Humidity is : ",round(predict[0][1],2))

st.write("<p style='font-size: 34px;'>Random Forest</p>",unsafe_allow_html=True)
randf=RandomForestRegressor()
randf.fit(X_train,Y_train)
prediction=randf.predict(X_test)
mean_error=mean_squared_error(Y_test,prediction)
print(mean_error)
predicted_randf=randf.predict(Input)
randf_score=randf.score(X_test,Y_test)
st.write("Accuracy is ",randf_score*100)
st.write("Temperature predicted is ",round(predicted_randf[0][0],2))
st.write("Humidity predicted is ",round(predicted_randf[0][1],2))
st.write("\n\n\n")

st.write("<p style='font-size: 34px;'>XGB Regressor</p>",unsafe_allow_html=True)
xgb_reg=XGBRegressor(n_estimators=100,random_state=42,learning_rate=0.1)
xgb_reg.fit(X_train,Y_train)
Y_pred=xgb_reg.predict(X_test)

score_xgb=xgb_reg.score(X_test,Y_test)
mean_squared_error(Y_pred,Y_test)
data = pd.DataFrame({"Day": [25], "Month": [6],"Hour": [23]})
prediction=model.predict(data)
st.write("Accuracy is ",score_xgb)
st.write("Temperature predicted is ",round(prediction[0][0],2))
st.write("Humidity predicted is",round(prediction[0][1],2))
