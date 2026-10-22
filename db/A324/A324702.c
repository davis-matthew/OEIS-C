/*
 * A324702  Lexicographically earliest sequence containing 2 and all positive
 *          integers > 1 whose prime indices minus 1 already belong to the
 *          sequence.
 * https://oeis.org/A324702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A324702 – Lexicographically earliest sequence containing 2 and all
    positive integers > 1 whose prime indices minus 1 already belong to
    the sequence.  The implementation is a direct search that keeps a
    record of the primes, their indices, and which numbers are already
    in the sequence.  Terms are memoised, so the function works for any
    order of calls. */
   
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*  Static state used by the routine.  All arrays are grown on demand. */
static struct {
    /*  Prime sieve data  */
    idx_t   max_limit;          /* Largest number currently sieved          */
    uint32_t *spf;              /* Smallest prime factor for each number    */
    uint32_t *prime_idx;        /* 1‑based index of each prime             */
    uint32_t *primes;           /* List of primes                          */
    size_t   prime_cnt;         /* Number of primes                        */

    /*  Sequence data  */
    seq_t   *seq_vals;          /* Array of sequence terms                 */
    size_t   seq_cap;           /* Capacity of seq_vals                     */
    size_t   seq_cnt;           /* Number of stored terms                   */

    /*  Membership map: 1 if the integer is in the sequence  */
    unsigned char *in_seq;      /* variable‑length bitmap                   */
    size_t   in_seq_cap;        /* Capacity of in_seq (max number indexed)   */

    /*  Next candidate to test  */
    idx_t     next_candidate;   

    bool      initialized;      /* Flag indicating first initialisation     */
} st = {0};

/*  Ensure that the sieve covers at least LIMIT.  */
static void
ensure_limit(size_t limit)
{
    if (limit <= st.max_limit) return;

    /*  Allocate new sieve arrays  */
    uint32_t *new_spf        = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *new_prime_idx  = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));

    size_t primes_cap = limit / 10 + 10;
    uint32_t *new_primes = (uint32_t*)calloc(primes_cap, sizeof(uint32_t));
    size_t prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                /* i is prime  */
            new_spf[i]        = (uint32_t)i;
            new_prime_idx[i]  = (uint32_t)(prime_cnt + 1);   /* 1‑based index */
            if (prime_cnt + 1 >= primes_cap) {              /* grow list   */
                primes_cap <<= 1;
                new_primes = (uint32_t*)realloc(new_primes, primes_cap * sizeof(uint32_t));
            }
            new_primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt &&
             (size_t)new_primes[j] <= new_spf[i] &&
             i * (size_t)new_primes[j] <= limit; ++j) {
            size_t prod = i * (size_t)new_primes[j];
            new_spf[prod] = new_primes[j];
        }
    }

    /*  Replace old arrays  */
    free(st.spf);
    free(st.prime_idx);
    free(st.primes);

    st.spf        = new_spf;
    st.prime_idx  = new_prime_idx;
    st.primes     = new_primes;
    st.prime_cnt  = prime_cnt;
    st.max_limit  = limit;
}

/*  Ensure that the membership bitmap covers (at least) VALUE.  */
static void
ensure_in_seq_cap(size_t need)
{
    if (need < st.in_seq_cap) return;
    size_t new_cap = st.in_seq_cap ? st.in_seq_cap << 1 : 1024;
    if (new_cap <= need) new_cap = need + 1;
    st.in_seq = (unsigned char*)realloc(st.in_seq, new_cap);
    memset(st.in_seq + st.in_seq_cap, 0, new_cap - st.in_seq_cap);
    st.in_seq_cap = new_cap;
}

/*  Append a term to the sequence array, growing it if necessary.  */
static void
append_seq_term(seq_t v)
{
    if (st.seq_cnt == st.seq_cap) {
        st.seq_cap = st.seq_cap ? st.seq_cap << 1 : 64;
        st.seq_vals = (seq_t*)realloc(st.seq_vals, st.seq_cap * sizeof(seq_t));
    }
    st.seq_vals[st.seq_cnt++] = v;
}

/*  Factorise N into its distinct prime factors.  The resulting primes
    are written into F[0..cnt-1].  The caller must supply an array large
    enough for all factors (at most st.prime_cnt).  */
static void
distinct_prime_factors(idx_t N, uint32_t *F, size_t *cnt)
{
    size_t k = 0;
    while (N > 1) {
        uint32_t p = st.spf[(size_t)N];
        F[k++] = p;
        while (N % p == 0) N /= p;
    }
    *cnt = k;
}

/*  Return the n‑th term of A324702.  */
seq_t
a(idx_t n)
{
    /*  First‑time initialisation  */
    if (!st.initialized) {
        /*  Small initial sieve – will grow automatically  */
        ensure_limit(2000);

        st.seq_vals   = (seq_t*)calloc(64, sizeof(seq_t));
        st.seq_cap    = 64;
        st.seq_cnt    = 0;

        st.in_seq     = (unsigned char*)calloc(32, sizeof(unsigned char));
        st.in_seq_cap = 32;

        /*  The mandatory first term is 2  */
        ensure_in_seq_cap(2);
        st.in_seq[2] = 1;
        append_seq_term((seq_t)2);

        st.next_candidate = 3;
        st.initialized = true;
    }

    /*  Generate terms until we have at least n of them  */
    while (st.seq_cnt < (size_t)n) {
        idx_t cand = st.next_candidate;

        /*  Extend sieve if necessary  */
        if ((size_t)cand > st.max_limit)
            ensure_limit((size_t)cand * 2 + 100);

        /*  Factorise the candidate  */
        uint32_t factors[64];
        size_t factor_cnt;
        distinct_prime_factors(cand, factors, &factor_cnt);

        bool ok = true;
        for (size_t i = 0; i < factor_cnt && ok; ++i) {
            uint32_t p = factors[i];
            uint32_t idx = st.prime_idx[p];
            if (idx <= 1) { ok = false; break; }  /* 2 cannot appear  */
            size_t need = (size_t)idx - 1;        /* prime index minus 1 */
            if (need >= st.in_seq_cap || st.in_seq[need] == 0) {
                ok = false; break;
            }
        }

        if (ok) {
            /*  Accept the candidate  */
            ensure_in_seq_cap((size_t)cand);
            st.in_seq[(size_t)cand] = 1;
            append_seq_term((seq_t)cand);
        }

        ++st.next_candidate;
    }

    return st.seq_vals[(size_t)n - 1];
}
