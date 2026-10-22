/*
 * A325100  Heinz numbers of strict integer partitions with no binary carries.
 * https://oeis.org/A325100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A325100 – Heinz numbers of strict integer partitions with no binary carries.
 *  The code generates the n‑th member by enumerating the numbers whose prime
 *  factor indices have no overlapping 1‑bits in binary (i.e. all prime
 *  indices are pairwise disjoint).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* allocates an array of seq_t elements */

/* ---------- Constants and static state ---------- */
#define LIMIT 2000000            /* sieve limit – enough for n <= 10 000 */
#define TERMS_CAP 20000          /* pre‑allocated capacity for results */

/* prime data */
static idx_t *spf = NULL;          /* smallest prime factor array */
static idx_t *primes = NULL;        /* array of primes */
static size_t primes_cnt = 0;       /* number of primes found */
static idx_t *prime_index = NULL;   /* map prime -> index (1‑based) */

/* sequence data */
static seq_t *terms = NULL;         /* array of computed terms */
static size_t terms_len = 0;        /* number of terms stored */

/* state for incremental generation */
static idx_t checked_upto = 0;      /* last integer examined */

/* ---------- Sieve and helpers ---------- */
static void build_sieve(void)
{
    if (spf) return;   /* already built */

    size_t n = (size_t)LIMIT;
    spf = (idx_t*)calloc(n + 1, sizeof(idx_t));
    primes = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    prime_index = (idx_t*)calloc(n + 1, sizeof(idx_t));

    size_t pc = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] &&
                            (idx_t)i * primes[j] <= LIMIT; ++j)
            spf[i * primes[j]] = primes[j];
    }
    primes_cnt = pc;

    for (size_t i = 0; i < primes_cnt; ++i)    /* index mapping (1‑based) */
        prime_index[primes[i]] = (idx_t)(i + 1);
}

/* ---------- Candidate validator ---------- */
static bool valid_candidate(idx_t n)
{
    if (n == 1) return true;                 /* empty partition */

    idx_t x = n;
    uint64_t used_bits = 0;

    while (x > 1) {
        idx_t p = spf[x];                    /* smallest prime factor */
        int exp = 0;
        while (x % p == 0) {                 /* count exponent */
            x /= p;
            exp++;
            if (exp > 1) return false;       /* not square‑free */
        }

        idx_t idx = prime_index[p];          /* prime index (1‑based) */
        if (idx == 0) return false;          /* safety check */

        if ((used_bits & idx) != 0) return false;   /* binary carry */
        used_bits |= idx;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* outside the offset */

    build_sieve();

    if (!terms) {                           /* allocate result array once */
        terms = seq_alloc(TERMS_CAP);
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        ++checked_upto;
        if (valid_candidate(checked_upto)) {
            terms[terms_len++] = (seq_t)checked_upto;
        }
    }

    return terms[n - 1];
}
