import React from 'react';
import './App.css';

import ClientJobs from './ClientJobs';
import ClientInfo from './ClientInfo';
import ActionMenu from './ActionMenu';

function ClientApp(props) {


    return (
        <div>
            <div>
                <div id='appHeader'>
                    URepair Web Application
                </div>
                <div id='leftCol'>
                    <ActionMenu
                        addJob={props.addJob}
                    />
                </div>
                <div id='centreCol'>
                    <ClientJobs
                        deleteJob={props.deleteJob}
                        fetchJobs={props.fetchJobs}
                        jobs={props.jobs}
                    />
                </div>
                <div id='rightCol'>
                    <ClientInfo
                        uid={props.uid}
                    />
                </div>
                <div>

                </div>
            </div>
        </div>

    )
}

export default ClientApp;