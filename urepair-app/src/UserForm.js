import React from 'react';
import './App.css';
import { useFormik } from 'formik';

function UserForm(props) {
   const formik = useFormik({
      initialValues: {
         name: '',
         email: '',
         address: '',
         phone: ''
      },
      onSubmit: values => {
         props.addJob(values.name, values.email, values.address, values.phone);
         props.enableLoginView();
      },
   });

   return (
      <form id="userform" onSubmit={formik.handleSubmit}>
         <label htmlFor="name">name</label>
         <input
            id="name"
            name="name"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.name}
         />

         <label htmlFor="email">email</label>
         <input
            id="email"
            name="email"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.email}
         />

         <label htmlFor="address">address</label>
         <input
            id="address"
            name="address"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.address}
         />

         <label htmlFor="phone">phone</label>
         <input
            id="phone"
            name="phone"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.phone}
         />
         <button type="submit">Submit</button>
      </form>
   );
};

export default UserForm;