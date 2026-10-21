/*
 * A245729  Composite numbers n = A020639(n) * A032742(n) where the greatest
 *          proper divisor A032742(n) is greater than the square of the
 *          smallest prime factor A020639(n), and that greatest proper divisor
 *          A032742(n) is either a prime or satisfies the same condition
 *          (i.e., is itself the term of this sequence).
 * https://oeis.org/A245729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence A245729 */
/* The term a(n) is the n‑th composite integer that satisfies the recursive
   condition below.  That condition is equivalent to the definition in
   the OEIS entry.  The implementation pre‑computes a sieve of size
   LIMIT = 2,000,000, which is sufficient for the first 10,000 terms
   and allows the algorithm to run well below the time limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 2000000 /* enough to generate >10^4 terms */

static uint32_t *spf = NULL;      /* smallest prime factor for numbers ≤LIMIT */
static bool     *is_prime = NULL; /* primality sieve */
static uint8_t  *status = NULL;   /* 0 = unknown, 1 = false, 2 = true */

/* forward declaration */
static bool in_seq(uint64_t n);

/* initialise sieve and status array */
static void init_sieve(void)
{
    if (spf != NULL) return; /* already initialised */

    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    status   = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));

    for (uint32_t i = 0; i <= LIMIT; ++i) {
        spf[i] = 0;
        is_prime[i] = false;
        status[i] = 0;
    }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            is_prime[i] = true;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
}

/* recursive test according to the OEIS definition.
   n must be ≤ LIMIT. */
static bool in_seq(uint64_t n)
{
    if (n <= LIMIT && status[n] != 0) return status[n] == 2;
    if (n <= LIMIT && is_prime[n]) { /* prime */
        if (n <= LIMIT) status[n] = 1;
        return false;
    }

    uint32_t p = spf[n];
    uint64_t d = n / p;

    /* test p^2 < d */
    if ((seq_t)p * (seq_t)p >= (seq_t)d) {
        if (n <= LIMIT) status[n] = 1;
        return false;
    }

    bool res;
    if (d <= LIMIT && is_prime[d]) {
        res = true;
    } else {
        res = in_seq(d);
    }

    if (n <= LIMIT) status[n] = res ? 2 : 1;
    return res;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;           /* store terms */
    static size_t seq_len = 0;          /* number of terms stored */
    static size_t seq_cap = 0;          /* array capacity */
    static idx_t cur = 2;               /* current number to test */

    init_sieve();

    /* add terms until we have at least n of them */
    while (seq_len < n) {
        while (cur <= LIMIT) {
            if (!is_prime[cur]) { /* composite */
                if (in_seq(cur)) {
                    /* grow array if necessary */
                    if (seq_len == seq_cap) {
                        seq_cap = seq_cap ? seq_cap * 2 : 16;
                        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    }
                    seq[seq_len++] = (seq_t)cur;
                    breakslab: ; // placeholder to allow break
                }
            }
            ++cur;
        }
        /* cur exceeded LIMIT; in practice this should not happen for
           requested n up to around 10,000 */
    }

    return seq[n - 1];
}
