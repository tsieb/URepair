import React, { useState } from 'react';
import './App.css';

import axios from "axios";


import ClientApp from './ClientApp';
import ContractorApp from './ContractorApp';


const client = axios.create({
   baseURL: "https://jsonplaceholder.typicode.com/posts"
});



function App() {
   const [waitingForUid, setWaitingForUid] = useState(false);
   const [isClient, setIsClient] = useState(true);

   //TODO - SET DEFAULT BACK TO AN EMPTY STRING
   const [uid, setUid] = useState('0');
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

   const handleInput = event => {
      setUid(event.target.value);
   };

   const enableContractorView = () => {
      setWaitingForUid(true);
      setIsClient(false);
   };

   const enableClientView = () => {
      setWaitingForUid(true);
      setIsClient(true);
   };

   return (
      <div className='App'>
         {
            waitingForUid ?
               isClient ?
                  <ClientApp
                     addJob={addJob}
                     deleteJob={deleteJob}
                     fetchJobs={fetchJobs}
                     jobs={jobs}
                     uid={uid}
                  />
                  :
                  <ContractorApp

                  />
               :
               <form>
                  <label>
                     User Identification Number :
                     <input type="number" value={uid} id='uidInput' onChange={handleInput} />
                  </label>
                  <button onClick={enableClientView}>Client</button>
                  <button onClick={enableContractorView}>Contractor</button>
               </form>
         }

      </div>
   );
};

export default App;
