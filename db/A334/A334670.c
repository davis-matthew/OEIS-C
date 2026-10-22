/*
 * A334670  a(n) = (2*n+1)!! * (Sum_{k=1..n} 1/(2*k+1)).
 * https://oeis.org/A334670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)!! * (Sum_{k=1..n} 1/(2*k+1)) Recurrence used:
 *            a(0)=0, a(n) = (2*n+1)*a(n-1) + (2*n-1)!! (n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  idx_t is an unsigned integer type; seq_t is an unsigned integer or big integer type.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy memoization of computed values */
    static seq_t *a_arr   = NULL;  /* stores a[0..max_n] */
    static idx_t  max_n   = 0;     /* highest n that has been computed */
    static seq_t f_prev   = (seq_t)1; /* (2*max_n-1)!!  ; for max_n==0  f_prev==1 !!   */

    if (n == 0)                /* a(0) is defined as 0 */
        return (seq_t)0;

    if (a_arr == NULL) {       /* first call: create array with a[0] */
        a_arr = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!a_arr) return (seq_t)0;            /* allocation failure   */
        a_arr[0] = (seq_t)0;
    }

    if (n <= max_n)            /* value already known */
        return a_arr[n];

    /* enlarge the array to hold up to a[n] */
    seq_t *tmp = (seq_t *)realloc(a_arr, sizeof(seq_t) * (n + 1));
    if (!tmp) return (seq_t)0;                      /* allocation failure   */
    a_arr = tmp;

    /* local copy of a[max_n] for speed */
    seq_t a_prev = a_arr[max_n];

    /* compute missing terms from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t four_mul_j_minus_two = (seq_t)(2 * i - 1);   /* (2*i-1)  */
        seq_t f_curr = f_prev * four_mul_j_minus_two;    /* (2*i-1)!! */
        seq_t two_i_plus_one = (seq_t)(2 * i + 1);
        seq_t a_curr = two_i_plus_one * a_prev + f_curr;  /* recurrence */
        a_arr[i] = a_curr;
        /* update for next round */
        a_prev = a_curr;
        f_prev = f_curr;
    }
    max_n = n;
    return a_arr[n];
}
