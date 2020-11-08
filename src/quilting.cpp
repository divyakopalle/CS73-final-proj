



// random placement of blocks

//neighboring blocks constrained by overlap

//mininmum error boundary cut

    // Dijkstra's

    //Dyanmic Programming 
    // want to make cut between overlapping blocks on pixels where the two textures match best
    // Allocate space to store error surfaces F[1:n,1:n]
    //traverse over all Blocks B2-BN (i=2...N) (why does paper say 2..N?)
        //for(int i=0;i<=n;i++){
           // for(int j=0;j<=n;j++){
               //if B1 and B2 are two blocks that overlap along their vertical edge
                // error_ij=(Biov-Bjov)^2
                // F[i][j]= error_ij+min(F[i-1][j-1],F[i-1][j],F[i-1][j+1])

          // }
       // }
       //minimum value of the last rown in F will indicate the end of the minimal vertical path through the surface
       //trace back to find the path of best cut
       //int j=n int i=1
       // vector 

        
           
