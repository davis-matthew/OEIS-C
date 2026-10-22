/*
 * A324701  Lexicographically earliest sequence containing 1 and all positive
 *          integers n such that the prime indices of n - 1 already belong to
 *          the sequence.
 * https://oeis.org/A324701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A324700(n) + 1. It is computed by generating the *
 *            lexicographically earliest sequence that contains 1 and,
 *            thereafter, * contains every positive integer n such that all
 *            prime indices of n‑1 * (indices of primes dividing n‑1) already
 *            appear in the sequence. * The sequence terms are produce...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* static lazy initialization ------------------------------------- */
    static bool init = false;
    static seq_t *seq = NULL;        /* sequence values (sorted) */
    static size_t seq_cap = 0;       /* capacity of seq array */
    static size_t seq_len = 0;       /* number of terms generated */

    static bool *incl = NULL;        /* incl[v] == true iff v is in seq */
    static size_t incl_cap = 0;      /* capacity of incl array */

    static idx_t *spf = NULL;        /* smallest prime factor table */
    static idx_t spf_limit = 0;      /* table valid for values 0..spf_limit */

    static idx_t *primes = NULL;     /* list of primes up to spf_limit */
    static size_t primes_cnt = 0;    /* number of primes */

    static idx_t *prime_index = NULL;/* prime_index[p] = index of prime p (1‑based) */
    static idx_t prime_index_cap = 0;/* capacity of prime_index array */

    static idx_t next_candidate = 2; /* next integer to test */
    /* -------------------------------------------------------------- */

    /* Initial build for a small safe limit (will grow if needed). */
    if (!init) {
        const idx_t initial_limit = 200000; /* safe upper bound for 10000 terms */

        /* spf table */
        build_spf(initial_limit, &spf);
        spf_limit = initial_limit;

        /* primes list */
        build_primes_upto(initial_limit, &primes, &primes_cnt);

        /* prime → index mapping */
        prime_index_cap = initial_limit + 1;
        prime_index = (idx_t*)calloc(prime_index_cap, sizeof(idx_t));
        for (size_t i = 0; i < primes_cnt; ++i) {
            prime_index[primes[i]] = (idx_t)(i + 1); /* indices are 1‑based */
        }

        /* inclusion array: values up to initial_limit */
        incl_cap = initial_limit + 1;
        incl = (bool*)calloc(incl_cap, sizeof(bool));

        /* sequence array */
        seq_cap = 1024;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 1;          /* sequence starts with 1 */
        seq_len = 1;
        incl[1] = true;      /* 1 is in the sequence */

        init = true;
    }

    /* Ensure we have enough terms for the requested index n. */
    while (seq_len < n) {
        idx_t m = next_candidate;    /* candidate value */
        next_candidate = (idx_t)(next_candidate + 1);

        /* enlarge tables if needed (m could be larger than current limits) */
        if (m > spf_limit) {
            idx_t new_limit = spf_limit * 2;
            if (new_limit < m + 10) new_limit = m * 2 + 10;

            /* rebuild spf */
            free(spf);
            build_spf(new_limit, &spf);
            spf_limit = new_limit;

            /* rebuild primes list and prime index map */
            free(primes);
            free(prime_index);
            build_primes_upto(new_limit, &primes, &primes_cnt);
            prime_index_cap = new_limit + 1;
            prime_index = (idx_t*)calloc(prime_index_cap, sizeof(idx_t));
            for (size_t i = 0; i < primes_cnt; ++i)
                prime_index[primes[i]] = (idx_t)(i + 1);
        }

        /* enlarge inclusion array if value exceeds its capacity */
        if (m >= incl_cap) {
            size_t new_cap = incl_cap * 2;
            if (new_cap < (size_t)m + 1) new_cap = (size_t)m + 1;
            incl = (bool*)realloc(incl, new_cap * sizeof(bool));
            memset(incl + incl_cap, 0, (new_cap - incl_cap) * sizeof(bool));
            incl_cap = new_cap;
        }

        /* Test whether all prime indices of m‑1 belong in the sequence. */
        bool forced = true;
        bool has_index = false;
        idx_t x = m - 1;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t idx = prime_index[p];
            has_index = true;
            if (!incl[idx]) {          /* index not yet in sequence */
                forced = false;
                /* skip remaining factorization */
                while (x % p == 0) x /= p;
                break;
            }
            while (x % p == 0) x /= p; /* skip multiplicity */
        }
        if (!has_index) forced = false; /* empty set is not enough */

        if (forced) {
            /* Append m to the sequence. */
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)m;
            incl[m] = true;
        }
    }

    return seq[n - 1];
}
