/*
 * A254078  a(n) is the number of steps after which n variables with
 *          increasing value ranges all have equal values when the values of
 *          all variables are decreased by 1 at each step and the value is set
 *          to the maximum value again when the resulting value would be 0.
 * https://oeis.org/A254078
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003418(n+1) - 2 (least common multiple of 1..n+1 minus
 *            2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* maximum n expected (per problem statement, up to 10000) */
#define MAX_K (10002)   /* store lcm(1..k) for k = 1 .. 10001, index k-1 */

static seq_t lcm_arr[MAX_K];
static bool lcm_initialized = false;

/* Euclidean algorithm for seq_t; uses modulo operator provided by seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute a(n) for n >= 2 (offset 2).  The function works for arbitrary
   seq_t types and is memoized across calls. */
seq_t a(idx_t n)
{
    /* initialize lcm array on first call, only once */
    if (!lcm_initialized) {
        /* lcm(1) = 1 */
        lcm_arr[0] = (seq_t)1;
        /* compute lcm(1..k) for k = 2 .. MAX_K-1 */
        for (idx_t k = 2; k < MAX_K; ++k) {
            seq_t prev_lcm = lcm_arr[k - 2];            /* lcm(1..k-1) */
            seq_t val = (seq_t)k;                      /* current number k */
            seq_t g = seq_gcd(prev_lcm, val);          /* gcd(prev_lcm, k) */
            seq_t part = prev_lcm / g;                 /* prev_lcm / gcd */
            lcm_arr[k - 1] = part * val;               /* lcm(1..k) */
        }
        lcm_initialized = true;
    }

    /* For n >= 2, we need lcm(1..n+1) stored at index (n+1)-1 = n */
    seq_t l = lcm_arr[n];   /* lcm(1..n+1) */
    return l - (seq_t)2;
}
