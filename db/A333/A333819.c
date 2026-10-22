/*
 * A333819  a(n) is the least integer q > 0 such that for some integer r,
 *          phi(q) + phi(r) = 2*n; where phi(n) is Euler's totient function
 *          (A000010).
 * https://oeis.org/A333819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A333819:  a(n) = least q>0 such that ∃r with φ(q)+φ(r)=2n
   (φ is Euler's totient). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_N 20000          /* 2*n for n<=10000 */
#define MAX_Q 200000         /* sieve limit for q and r */

static bool initialized = false;
static seq_t *phi = NULL;               /* φ sieve storage */
static bool totient_exists[MAX_N + 1];   /* true if value occurs as φ(r) */

/* Helper to initialize the sieve of φ values and the totient lookup table */
static void init_phi_sieve(void)
{
    size_t i, j;
    phi = (seq_t *)malloc((MAX_Q + 1) * sizeof(seq_t));
    if (!phi) abort();  /* memory error */

    for (i = 0; i <= MAX_Q; ++i)
        phi[i] = (seq_t)i;

    for (i = 2; i <= MAX_Q; ++i) {
        if (phi[i] == (seq_t)i) {          /* i is prime */
            for (j = i; j <= MAX_Q; j += i) {
                phi[(size_t)j] -= phi[(size_t)j] / (seq_t)i;
            }
        }
    }

    /* mark which numbers up to MAX_N occur as φ(r) */
    for (i = 1; i <= MAX_Q; ++i) {
        uint64_t tv = (uint64_t)phi[i];
        if (tv <= MAX_N)
            totient_exists[(size_t)tv] = true;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_phi_sieve();

    seq_t target = (seq_t)n * (seq_t)2;   /* 2n */
    seq_t q_val;
    for (q_val = 1; (seq_t)q_val <= (seq_t)MAX_Q; ++q_val) {
        seq_t phi_q = phi[(size_t)q_val];
        if (phi_q >= target)   /* X would be negative or zero */
            continue;
        seq_t X = target - phi_q;
        if (X < 1) continue;
        if (X <= (seq_t)MAX_N && totient_exists[(size_t)X])
            return q_val;
    }
    /* According to the conjecture a solution always exists, but
       if not found return 0 to signal error */
    return 0;
}
