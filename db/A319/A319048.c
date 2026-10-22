/*
 * A319048  a(n) is the greatest k such that A000010(k) divides n where
 *          A000010 is the Euler totient function.
 * https://oeis.org/A319048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max_{d|n} (maximum k with phi(k)=d)
 * Range:     u8 n <= 71, u16 n <= 9405, u32 n <= 9405, u64 n <= 9405,
 *            u128 n <= 9405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Maximum k to pre‑compute (sufficient for n up to about 10 000) */
static const size_t MAX_K = 5000000U;

/* Global data, built once */
static bool            precomp_done   = false;
static uint64_t *      phi_array      = NULL;     /* phi for 1..MAX_K, type uint64_t */
static seq_t *         maxk_of_phi    = NULL;     /* max k with phi(k)=d, stored by d */

/* Helper: one‑time pre‑computation */
static void precalc(void)
{
    if (precomp_done)
        return;

    /* Allocate phi array */
    phi_array = (uint64_t*)malloc((MAX_K + 1) * sizeof(uint64_t));
    if (!phi_array) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= MAX_K; ++i)
        phi_array[i] = (uint64_t)i;

    /* Sieve for totients */
    for (size_t i = 2; i <= MAX_K; ++i)
        if (phi_array[i] == i) {          /* i is prime */
            for (size_t j = i; j <= MAX_K; j += i)
                phi_array[j] -= phi_array[j] / i;
        }

    /* Allocate maxk_of_phi array */
#ifdef __cplusplus
    /* C++: use new to default‑construct seq_t objects */
    maxk_of_phi = new seq_t[MAX_K + 1];
#else
    /* C: malloc and zero‑initialize */
    maxk_of_phi = (seq_t*)calloc((MAX_K + 1), sizeof(seq_t));
    if (!maxk_of_phi) {
        free(phi_array);
        exit(EXIT_FAILURE);
    }
#endif

    /* Populate maxk_of_phi */
    for (size_t k = 1; k <= MAX_K; ++k) {
        uint64_t phi_val = phi_array[k];
        if (phi_val <= MAX_K) {
            seq_t k_val = (seq_t)k;
            if (maxk_of_phi[phi_val] < k_val)
                maxk_of_phi[phi_val] = k_val;
        }
    }

    /* Clean up temporary array */
    free(phi_array);
    precomp_done = true;
}

/* --------------------------------------------------------------- */
/*  seq_t a(idx_t n)  */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    precalc();

    seq_t best = (seq_t)0;

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if ((size_t)d <= MAX_K) {
                seq_t val = maxk_of_phi[d];
                if (val > best)
                    best = val;
            }

            /* corresponding divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && (size_t)d2 <= MAX_K) {
                seq_t val = maxk_of_phi[d2];
                if (val > best)
                    best = val;
            }
        }
    }

    return best;
}
