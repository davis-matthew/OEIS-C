/*
 * A389227  Numbers in A126706 that are less than the square of their
 *          squarefree kernel.
 * https://oeis.org/A389227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer n such that * 1 < n / rad(n) <
 *            rad(n) and rad(n) is composite (product of at least two distinct
 *            primes). * Here rad(n) is the squarefree kernel of n. *
 *            (Equivalently: n = m * k where k = rad(n) is composite
 *            squarefree, * 1 < m < k, and every prime divisor of m div...
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Assumes seq_t and idx_t are defined elsewhere in the build. */

/* --------- Configuration ---------- */
#define LIMIT 5000000u  /* maximum n to sieve; should be large enough for n <= 10000 */

/* --------- Static data ----------- */
static bool     inited   = false;
static int      *spf      = NULL;   /* smallest prime factor (int) */
static uint32_t *rad_arr  = NULL;   /* rad(n) for n up to LIMIT */
static bool     *is_prime = NULL;
static seq_t   *seq      = NULL;
static size_t   seq_len  = 0;
static size_t   seq_cap  = 0;
static idx_t    last_checked = 1;   /* last n examined in the scan */

/* --------- Helper functions --------- */
static void init(void)
{
    if (inited) return;

    /* Allocate arrays */
    spf      = (int*)calloc((size_t)LIMIT + 1, sizeof(int));
    rad_arr  = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    is_prime = (bool*)calloc((size_t)LIMIT + 1, sizeof(bool));

    /* Linear sieve for smallest prime factor */
    int *primes = (int*)malloc((LIMIT / 10) * sizeof(int));
    size_t pcnt = 0;
    for (int i = 2; i <= (int)LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt && (long long)primes[j] <= spf[i] &&
             (long long)i * primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* Compute rad(n) */
    rad_arr[1] = 1;
    for (int i = 2; i <= (int)LIMIT; ++i) {
        int p = spf[i];
        int m = i / p;
        if (m % p == 0)
            rad_arr[i] = rad_arr[m];
        else
            rad_arr[i] = rad_arr[m] * (uint32_t)p;
    }

    /* Prime flags */
    for (int i = 2; i <= (int)LIMIT; ++i)
        is_prime[i] = (spf[i] == i);

    /* Initialize sequence storage */
    seq_cap = 128;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));

    /* Clean up temporary primes array */
    free(primes);

    inited = true;
}

/* Append a value to the dynamic seq array */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap <<= 1;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* --------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!inited) init();

    /* Generate terms until we reach the requested index */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (last_checked > (idx_t)LIMIT) {
            /* In the unlikely event the precomputed limit is insufficient,
               just return 0. This should not happen for n <= 10000. */
            return (seq_t)0;
        }

        uint32_t k = rad_arr[last_checked];            /* rad(n) */
        if (k <= 1) continue;                          /* skip n = 1 */

        if (is_prime[k]) continue;                     /* need composite squarefree */

        idx_t m = last_checked / k;                    /* integer division, always exact */

        if (m <= 1 || m >= (idx_t)k) continue;         /* need 1 < m < k */

        /* Check that every prime divisor of m divides k (i.e., rad(m) | k) */
        uint32_t temp_m = (uint32_t)m;
        bool ok = true;
        while (temp_m > 1) {
            int p = spf[temp_m];
            if (k % p != 0) {                          /* prime not in k */
                ok = false;
                break;
            }
            while (temp_m % p == 0)
                temp_m /= p;
        }
        if (!ok) continue;

        /* All conditions satisfied; add to sequence */
        append_seq((seq_t)last_checked);
    }

    return seq[n-1];
}
