import React from 'react';
import './App.css';
import { useFormik } from 'formik';

function JobForm(props) {
   const formik = useFormik({
      initialValues: {
         title: '',
         loc: '',
         time_start: '',
         time_end: '',
         desc: '',
         price: ''
      },
      onSubmit: values => {
         alert(JSON.stringify(values, null, 2));
         props.addJob(values.title, values.loc, values.times, values.timee, values.desc, values.price)
      },
   });

   return (
      <form id="jobform" onSubmit={formik.handleSubmit}>
         <label htmlFor="title">Title</label>
         <input
            id="title"
            name="title"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.title}
         />

         <label htmlFor="location">location</label>
         <input
            id="location"
            name="location"
            type="text"
            onChange={formik.handleChange}
            value={formik.values.location}
         />

         <label htmlFor="time_start">Time from</label>
         <input
            id="time_start"
            name="time_start"
            type="time"
            onChange={formik.handleChange}
            value={formik.values.time_start}
         />

         <label htmlFor="time_end">to</label>
         <input
            id="time_end"
            name="time_end"
            type="time"
            onChange={formik.handleChange}
            value={formik.values.time_end}
         />

         <label htmlFor="desc">Description</label>
         <textarea
            id="desc"
            name="desc"
            type="desc"
            onChange={formik.handleChange}
            value={formik.values.desc}
         />

         <label htmlFor="price">price</label>
         <input
            id="price"
            name="price"
            type="price"
            onChange={formik.handleChange}
            value={formik.values.price}
         />

         <button type="submit">Submit</button>
      </form>
   );
};

export default JobForm;