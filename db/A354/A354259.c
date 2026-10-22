/*
 * A354259  Expansion of e.g.f. 1/sqrt(1 - 6 * log(1+x)).
 * https://oeis.org/A354259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} (3/2)^k * (2*k)! * Stirling1(n, k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code iteratively builds the Stirling numbers of the first kind s(n,k)
   and accumulates the required sum using the recurrence                                   */
/*   a_{n+1} = sum_{k=0}^{n-1} (3*(2*k+1))*T_k*s(n,k) + T_{n+1} - n*a_n               */
/* where T_k = (3/2)^k * (2k)! / k!                                                             */
/* The algorithm uses only integer arithmetic and works for any seq_t that supports    */
/* +, -, *, and assignment.                                                                    */

#include <stdbool.h>
#include "../utils/alloc_seq.h"

/* Maximum number of terms that may be asked for (safe > 10000) */
#define MAX_CAP 20001

static bool         initialized  = false;
static seq_t        *a_cache      = NULL;   /* a[0..current_n_done]          */
static seq_t        *term         = NULL;   /* T_k * s(current_n, k)         */
static idx_t        current_n_done = 0;     /* highest n that has been processed */
static seq_t        T_curr         = 0;     /* T_n for the current n          */
static seq_t        a_curr         = 0;     /* a_n for the current n          */

static void init(void)
{
    /* Allocate space for the maximum possible number of terms */
    a_cache = alloc_seq(MAX_CAP);
    term    = alloc_seq(MAX_CAP);
    /* initial values */
    a_cache[0] = 1;
    term[0]    = 1;         /* T_0 * s(0,0) = 1 * 1 */
    T_curr     = 1;         /* T_0 = 1                           */
    a_curr     = 1;         /* a_0 = 1                           */
    current_n_done = 0;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* Extend the precomputed range up to n */
    while (current_n_done < n) {
        idx_t cur = current_n_done;          /* current n */

        /* 1. sum_{k=0}^{cur-1} r_k * term_k ,   r_k = 3*(2k+1) */
        seq_t sum_r_term = 0;
        for (idx_t k = 0; k < cur; ++k) {
            seq_t r_k = (seq_t)3 * ((seq_t)(2 * k) + 1);
            sum_r_term += term[k] * r_k;
        }

        /* 2. compute T_{cur+1} = T_cur * (3*(2*cur+1)) */
        seq_t r_next = (seq_t)3 * ((seq_t)(2 * cur) + 1);
        seq_t T_next = T_curr * r_next;

        /* 3. compute a_{cur+1} = sum_r_term + T_next - cur*a_cur */
        a_curr = sum_r_term + T_next - (seq_t)cur * a_curr;
        ++current_n_done;
        a_cache[current_n_done] = a_curr;
        T_curr = T_next;

        /* 4. update term array to represent T_k*s_{cur+1}(k) */
        /*    First update k from cur down to 1 */
        for (idx_t k = cur; k >= 1; --k) {
            seq_t term_k_minus1 = term[k - 1];
            seq_t term_k_val    = term[k];
            seq_t r_prev        = (seq_t)3 * (((seq_t)(2 * (k - 1)) + 1));
            term[k] = term_k_minus1 * r_prev - (seq_t)cur * term_k_val;
        }
        term[0]          = 0;      /* s_{cur+1}(0) = 0 */
        term[cur + 1]    = T_next; /* s_{cur+1}(cur+1) = 1, so term = T_{cur+1} */
    }

    return a_cache[n];
}
