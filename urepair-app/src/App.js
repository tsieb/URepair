import React, { useState } from 'react';
import axios from "axios";
import './App.css';
import { Formik, Form, Field, ErrorMessage } from 'formik';
import { useFormik } from 'formik';


const client = axios.create({
   baseURL: "https://jsonplaceholder.typicode.com/posts"
});


function PostForm(props) {
   const formik = useFormik({
      initialValues: {
         title: '',
         loc: '',
         time_start: '',
         time_end: '',
         desc: ''
      },
      onSubmit: values => {
         alert(JSON.stringify(values, null, 2));
         props.addPost(values.title, values.loc, values.times, values.timee, values.desc)
      },
   });

   return (
      <form onSubmit={formik.handleSubmit}>
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

         <button type="submit">Submit</button>

      </form>

   );

};



function ActionMenu(props) {

   return (
      <div className='ActionMenu'>
         <div className='border'>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <button>
               button
            </button>
            <PostForm 
               addPost={props.addPost}
            />
         </div>
      </div>
   )
}

function ClientInfo(props) {

   return (
      <div className='ClientInfo'>
         <div className='border'>
            <header>
               Name
            </header>
            <h4>
               Contact info
            </h4>
         </div>
      </div>
   )
}


function ClientJobs(props) {

   return (
      <div className="ClientJobs">
         <div className='border'>
            <div>
               <button onClick={() => props.fetchPost()}>Get new posts</button>
            </div>
            <div>0.

               {props.posts.map((post) => {
                  return (
                     <div className="post-card" key={post.id}>
                        #{post.id} - {post.title}
                        <div className="button">
                           <div className="delete-btn" onClick={() => props.deletePost(post.id)}>
                              {post.body}
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



function App() {

   const [title, setTitle] = useState('');
   const [loc, setLoc] = useState('');
   const [times, setTimes] = useState('');
   const [timee, setTimee] = useState('');
   const [desc, setDesc] = useState('');
   const [posts, setPosts] = useState([]);

   // GET with Axios
   const fetchPost = async () => {
      let response = await client.get('?_limit=20');
      setPosts(response.data);
   };

   // Delete with Axios
   const deletePost = async (id) => {
      await client.delete(`${id}`);
      setPosts(
         posts.filter((post) => {
            return post.id !== id;
         })
      );
   };

   // Post with Axios
   const addPost = async (title, loc, times, timee, desc) => {
      let response = await client.post('', {
         title: title,
         loc: loc,
         times: times,
         timee: timee,
         desc: desc,
      });
      setPosts((posts) => [response.data, ...posts]);
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
               addPost={addPost}
               setTitle={setTitle}
               setLoc={setLoc}
               setTimes={setTimes}
               setTimee={setTimee}
               setDesc={setDesc}
            />
         </div>
         <div id='centreCol'>
            <ClientJobs
               deletePost={deletePost}
               fetchPost={fetchPost}
               posts={posts}
            />
         </div>
         <div id='rightCol'>
            <ClientInfo />
         </div>
      </div>
   );
};

export default App;
