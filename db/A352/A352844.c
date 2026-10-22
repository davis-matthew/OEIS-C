/*
 * A352844  Smallest k > 1 such that sopfr(k) - tau(k) = n, or -1 if no such k
 *          exists.
 * https://oeis.org/A352844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A352844:
   a(n) = the smallest integer k > 1 such that sopfr(k) - tau(k) = n,
   where sopfr(k) is the sum of the prime factors of k
   (with multiplicity) and tau(k) is the number of positive divisors of k.
   If no such k exists, the function returns -1.  */

/* Compilation Requirements:
   This code uses only the standard C headers allowed in the specification.
   The type seq_t and idx_t are supplied by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t const LIMIT = 600000;                     /* pre‑compute up to this limit – enough for n ≤ 100 000 */
static uint32_t *spf = NULL;                            /* smallest prime factor array */
static seq_t    *sop = NULL;                            /* sopfr array */
static uint32_t *tau_arr = NULL;                        /* divisor count array */
static bool initialized = false;

/* -------------------------------------------------------------
   Build the smallest prime factor sieve and pre‑compute sop and tau.
   ------------------------------------------------------------- */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate arrays */
    spf      = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    sop      = (seq_t*)   malloc((LIMIT + 1) * sizeof(seq_t));
    tau_arr  = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf || !sop || !tau_arr) abort();              /* out of memory */

    /* Small‑prime‑factor sieve */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * (size_t)i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Pre‑compute sopfr (sum of prime factors, counted with multiplicity)
       and tau (divisor count). */
    sop[0] = sop[1] = 0;
    tau_arr[0] = tau_arr[1] = 1;
    for (size_t n = 2; n <= LIMIT; ++n) {
        size_t x = n;
        seq_t sum = 0;
        uint32_t divs = 1;
        while (x > 1) {
            uint32_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                sum += (seq_t)p;
            }
            divs *= (uint32_t)(cnt + 1);
        }
        sop[n] = sum;
        tau_arr[n] = divs;
    }
}

/* -------------------------------------------------------------
   Public function required by the runner.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                                  /* ensure tables are built */

    seq_t n_seq = (seq_t)n;                  /* convert query index to seq_t for comparison */

    /* Search for the smallest k with sopfr(k) - tau(k) == n. */
    for (size_t k = 2; k <= LIMIT; ++k) {
        seq_t diff = sop[k] - (seq_t)tau_arr[k];
        if (diff == n_seq) {
            return (seq_t)k;
        }
    }

    /* If we did not find anything within the pre‑computed range,
       we can safely extend the limit (not necessary for n ≤ 10000)
       or return -1. */
    return (seq_t)-1;
}
