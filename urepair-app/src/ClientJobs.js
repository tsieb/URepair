import React from 'react';
import './App.css';

function ClientJobs(props) {

    return (
       <div className="ClientJobs">
          <div className='border'>
             <div>
                <button onClick={() => props.fetchJobs()}>Get new posts</button>
             </div>
             <div>
 
                {props.jobs.map((job) => {
                   return (
                      <div className="post-card" key={job.id}>
                         #{job.id} - {job.title}
                         <div className="button">
                            <div className="delete-btn" onClick={() => props.deleteJob(job.id)}>
                               {job.body}
                            </div>
                         </div>
                      </div>
                   );
                })}
             </div>
          </div>
       </div>
 
    )
 
 
 
 }

 export default ClientJobs;