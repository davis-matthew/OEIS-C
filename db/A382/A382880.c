/*
 * A382880  Symmetric triangle read by rows refining A109113.
 * https://oeis.org/A382880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient at position n in the symmetric triangle read
 *            by rows defined by the recurrence f_0(y) = 1 + y f_1(y) = 1 + 6y
 *            + 6y² + y³ f_n(y) = (1 + 4y + y²) f_{n-1}(y) + (y + y² + y³)
 *            f_{n-2}(y) (n ≥ 2) The row r (r ≥ 0) has 2(r+1) coefficients;
 *            a(n) is the coefficient whose global index n...
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, free */
 
#ifndef __cplusplus
static seq_t **_rows   = NULL;   /* _rows[r] points to the coefficients of f_r */
static size_t *_len_rows = NULL; /* length of each row array */
static idx_t  _max_rows   = 0;    /* number of rows currently cached   */
#else
#include <vector>
static std::vector< std::vector<seq_t> > _rows; /* vector of rows */
#endif
 
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Find the row r such that (r+1)(r+2) > n.  */
    idx_t r = 0;
    while ( (__uint128_t)(r + 1) * (r + 2) <= n ) ++r;
    idx_t target = r;                         /* row index containing n   */
 
    /* Grow the cache if necessary.   */
    if ( target >= static_cast<idx_t>(_rows.size()) ) {
        size_t old_size = _rows.size();
        _rows.resize( target + 1 );
        for ( size_t i = old_size; i <= (size_t)target; ++i ) {
            size_t len = 2 * (i + 1);
            _rows[i].assign( len, (seq_t)0 );
            if ( i == 0 ) {            /* f_0(y) = 1 + y               */
                _rows[i][0] = (seq_t)1;
                _rows[i][1] = (seq_t)1;
            } else if ( i == 1 ) {     /* f_1(y) = 1 + 6y + 6y² + y³   */
                _rows[i][0] = (seq_t)1;
                _rows[i][1] = (seq_t)6;
                _rows[i][2] = (seq_t)6;
                _rows[i][3] = (seq_t)1;
            } else {                   /* recurrence for i ≥ 2          */
                const std::vector<seq_t> &prev      = _rows[i-1];
                const std::vector<seq_t> &prevprev  = _rows[i-2];
                /* (1 + 4y + y²) * prev                     */
                for ( size_t k = 0; k < prev.size(); ++k ) {
                    _rows[i][k]     += prev[k];
                    _rows[i][k + 1] += prev[k] * (seq_t)4;
                    _rows[i][k + 2] += prev[k];
                }
                /* (y + y² + y³) * prevprev                */
                for ( size_t k = 0; k < prevprev.size(); ++k ) {
                    _rows[i][k + 1] += prevprev[k];
                    _rows[i][k + 2] += prevprev[k];
                    _rows[i][k + 3] += prevprev[k];
                }
            }
        }
    }
 
    /* Map global index n to position inside the row.          */
    idx_t start = (idx_t)target * (target + 1); /* first index of the row */
    idx_t pos   = n - start;
    return _rows[target][pos];
#else
    /* Same logic as above, but using raw pointers instead of std::vector. */
    idx_t r = 0;
    while ( (__uint128_t)(r + 1) * (r + 2) <= n ) ++r;
    idx_t target = r;                          /* row index containing n   */
 
    /* Grow the cache if necessary.   */
    if ( target >= _max_rows ) {
        size_t new_size = target + 1;
        _rows = (seq_t**)realloc( _rows, new_size * sizeof(seq_t*) );
        _len_rows = (size_t*)realloc( _len_rows, new_size * sizeof(size_t) );
        for ( idx_t i = _max_rows; i < (idx_t)new_size; ++i ) {
            size_t len = 2 * (i + 1);
            seq_t *arr = (seq_t*)calloc( len, sizeof(seq_t) );
            _len_rows[i] = len;
 
            if ( i == 0 ) {              /* f_0(y) = 1 + y               */
                arr[0] = (seq_t)1;
                arr[1] = (seq_t)1;
            } else if ( i == 1 ) {       /* f_1(y) = 1 + 6y + 6y² + y³   */
                arr[0] = (seq_t)1;
                arr[1] = (seq_t)6;
                arr[2] = (seq_t)6;
                arr[3] = (seq_t)1;
            } else {                     /* recurrence for i ≥ 2          */
                seq_t *prev      = _rows[i-1];
                seq_t *prevprev  = _rows[i-2];
                size_t len_prev      = _len_rows[i-1];
                size_t len_prevprev  = _len_rows[i-2];
                /* (1 + 4y + y²) * prev                     */
                for ( size_t k = 0; k < len_prev; ++k ) {
                    arr[k]     += prev[k];
                    arr[k + 1] += prev[k] * (seq_t)4;
                    arr[k + 2] += prev[k];
                }
                /* (y + y² + y³) * prevprev                */
                for ( size_t k = 0; k < len_prevprev; ++k ) {
                    arr[k + 1] += prevprev[k];
                    arr[k + 2] += prevprev[k];
                    arr[k + 3] += prevprev[k];
                }
            }
            _rows[i] = arr;
        }
        _max_rows = new_size;
    }
 
    /* Map global index n to position inside the row.          */
    idx_t start = target * (target + 1); /* first index of the row */
    idx_t pos   = n - start;
    return _rows[target][pos];
#endif
}
