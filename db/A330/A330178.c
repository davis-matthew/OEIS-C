/*
 * A330178  a(n) = n + floor(nr/t) + floor(ns/t), where r = e - 2, s = e - 1,
 *          t = e.
 * https://oeis.org/A330178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*r/t) + floor(n*s/t), where r = e - 2, s = e -
 *            1, t = e. Equivalent to a(n) = 3n - floor(2n/e) - floor(n/e) -
 *            2. The function uses convergents of the continued fraction of
 *            1/e to compute the exact floor values without floating point
 * Range:     u8 n <= 52, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_CONV 60  /* enough for any idx_t up to 2^64-1 */

static int cf[MAX_CONV];
static uint64_t convP[MAX_CONV]; /* numerator of the convergents */
static uint64_t convQ[MAX_CONV]; /* denominator of the convergents */
static int  conv_init_done = 0;

/* Initialise the continued fraction coefficients for 1/e
   and their convergents.  The pattern for 1/e is
   [0; 2, 1, 2, 1, 1, 4, 1, 1, 6, 1, 1, 8, 1, 1, 10, …]   */
static void init_convergents(void)
{
    if (conv_init_done) return;
    /* build the coefficient array */
    cf[0] = 0;          /* 1/e starts with 0 */
    cf[1] = 2;          /* the second coefficient is 2 */
    int idx = 2;

    /* the pattern after the initial two terms:
       1, 2  followed by blocks 1,1,4 ; 1,1,6 ; 1,1,8 ; …   */
    if (idx < MAX_CONV) cf[idx++] = 1;   /* 1, 2, 1, 1, 4, 1, 1, 6, … */
    if (idx < MAX_CONV) cf[idx++] = 2;
    int k = 2;  /* starts from 4 = 2*k at the first block */
    while (idx < MAX_CONV) {
        if (idx < MAX_CONV) cf[idx++] = 1;
        if (idx < MAX_CONV) cf[idx++] = 1;
        if (idx < MAX_CONV) cf[idx++] = 2 * (uint64_t)k;
        ++k;
    }

    /* compute convergents using the recurrence:
       h_i = a_i * h_{i-1} + h_{i-2}
       k_i = a_i * k_{i-1} + k_{i-2}   */
    __int128 h_m2 = 0, h_m1 = 1;   /* h[-2] = 0, h[-1] = 1 */
    __int128 k_m2 = 1, k_m1 = 0;   /* k[-2] = 1, k[-1] = 0 */

    for (int i = 0; i < MAX_CONV; ++i) {
        __int128 h = (__int128)cf[i] * h_m1 + h_m2;
        __int128 k = (__int128)cf[i] * k_m1 + k_m2;
        convP[i] = (uint64_t)h;
        convQ[i] = (uint64_t)k;
        h_m2 = h_m1; h_m1 = h;
        k_m2 = k_m1; k_m1 = k;
    }
    conv_init_done = 1;
}

/* Exact integer floor of N / e, where N is any non‑negative integer.   */
static seq_t floor_by_e_seq(const seq_t N)
{
    init_convergents();

    /* convergents alternate around 1/e:
       convergents with even index < 1/e, odd index > 1/e.       */
    for (int i = 2; i + 1 < MAX_CONV; i += 2) {
        seq_t p_low  = (seq_t)convP[i];     /* p/q  < 1/e          */
        seq_t q_low  = (seq_t)convQ[i];
        seq_t p_up   = (seq_t)convP[i - 1]; /* p/q  > 1/e          */
        seq_t q_up   = (seq_t)convQ[i - 1];

        seq_t low  = (N * p_low)  / q_low;   /* <= floor(N/e)       */
        seq_t high = (N * p_up)   / q_up;    /* >= floor(N/e)       */
        if (low == high) return low;          /* exact value found   */
    }

    /* The loop above always terminates for any N because the
       continued fraction convergents converge arbitrarily well.
       The following is a defensive fallback (unlikely to run). */

    /* use the last available convergent as a lower bound */
    seq_t p_low  = (seq_t)convP[MAX_CONV - 2];
    seq_t q_low  = (seq_t)convQ[MAX_CONV - 2];
    return (N * p_low) / q_low;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* convert index to seq_t   */
    seq_t floor_n  = floor_by_e_seq(N);
    seq_t floor_2n = floor_by_e_seq(N * (seq_t)2);
    return N * (seq_t)3 - floor_2n - floor_n - (seq_t)2;
}
