import React, { useState } from 'react';
import './App.css';

import axios from "axios";


import ClientApp from './ClientApp';
import ContractorApp from './ContractorApp';
import UserForm from './UserForm';


const client = axios.create({
   baseURL: "http://localhost:80/"
});



function App() {

   const [waitingForUid, setWaitingForUid] = useState(false);
   const [isClient, setIsClient] = useState(true);
   const [waitingForNew, setWaitingForNew] = useState(false);
   const [isNew, setIsNew] = useState(true);

   //TODO - SET DEFAULT BACK TO AN EMPTY STRING
   const [uid, setUid] = useState('0');
   const [jobs, setJobs] = useState([]);



   const fetchJobs = async () => {
      let response = await client.get('/userjob/' + uid);
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

   const addJob = async (title, loc, times, timee, desc, price) => {
      let response = await client.post('/job', {
         title: title,
         location: loc,
         start_time: times,
         end_time: timee,
         description: desc,
         price: price,
         userid: uid,
         contractorid: -1
      });
      setJobs((jobs) => [response.data, ...jobs]);
   };

   const addUser = async (name, email, address, phone) => {
      let response = await client.post('/user', {
         name: name,
         email: email,
         address: address,
         phone: phone,
      });
      setJobs((jobs) => [response.data, ...jobs]);
   };

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

   const enableCreateView = () => {
      setWaitingForNew(true);
      setIsNew(false);
   };

   const enableLoginView = () => {
      setWaitingForNew(true);
      setIsNew(true);
   };

   return (
      <div className='App'>
         {
            waitingForNew ?
               isNew ?
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
                  :
                  <UserForm
                     addUser={addUser}
                     enableLoginView={enableLoginView}
                  />
               :
               <>
                  <label>
                     Is this your first time visiting the site? 
                  </label>
                  <button onClick={enableCreateView}>Create Account</button>
                  <button onClick={enableLoginView}>Login</button>
               </>
         }

      </div>
   );
};

export default App;
