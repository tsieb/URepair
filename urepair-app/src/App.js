import React, { useState } from 'react';
import './App.css';

import axios from "axios";

import ClientJobs from './ClientJobs';
import ClientInfo from './ClientInfo';
import ActionMenu from './ActionMenu';


const client = axios.create({
   baseURL: "https://jsonplaceholder.typicode.com/posts"
});



function App() {
   const [uid, setUid] = useState('');

   const [jobs, setJobs] = useState([]);


   const fetchJobs = async () => {
      let response = await client.get('?_limit=5');
      setJobs(response.data);
   };


   const deleteJob = async (id) => {
      await client.delete(`${id}`);
      setJobs(
         jobs.filter((job) => {
            return job.id !== id;
         })
      );
   };

   const addJob = async (title, loc, times, timee, desc) => {
      let response = await client.post('', {
         title: title,
         loc: loc,
         times: times,
         timee: timee,
         desc: desc,
      });
      setJobs((jobs) => [response.data, ...jobs]);
   };
   /*
   const handleSubmit = (e) => {
      e.preventDefault();
      addPost(title, body);
   };
   */

   return (
      <div className='App'>
         <div id='appHeader'>
            Logo and top bar
         </div>
         <div id='leftCol'>
            <ActionMenu
               addJob={addJob}
            />
         </div>
         <div id='centreCol'>
            <ClientJobs
               deleteJob={deleteJob}
               fetchJobs={fetchJobs}
               jobs={jobs}
            />
         </div>
         <div id='rightCol'>
            <ClientInfo 
               setUid={setUid}
            />
         </div>
      </div>
   );
};

export default App;
