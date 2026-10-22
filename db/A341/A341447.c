/*
 * A341447  Heinz numbers of integer partitions whose only even part is the
 *          smallest.
 * https://oeis.org/A341447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th Heinz number of integer partitions whose only
 *            even part is the smallest. This implementation generates the
 *            sequence by testing integers in increasing order and selecting
 *            those whose prime factorisation satisfies: • exactly one
 *            distinct even-indexed prime factor (with exponent 1) •...
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* constant upper bound for sieve – large enough to produce >10000 terms. */
static const size_t SIEVE_LIMIT = 5000000u;

/* Global data structures used for memoisation. */
static uint32_t *spf = NULL;          /* smallest prime factor for each number */
static uint32_t *prime_index = NULL;   /* prime number -> its 1‑based index */
static uint32_t *primes = NULL;        /* list of primes produced by the sieve */

static seq_t *seq = NULL;              /* cached sequence values */
static size_t seq_len = 0, seq_cap = 0;
static idx_t next_val = 2;             /* next integer to test */

/*--------------------------------------------------------------------
   Initialise sieve and cached sequence.
--------------------------------------------------------------------*/
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* ---------- Sieve for spf and prime indices ---------- */
    spf = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    prime_index = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    primes = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = (uint32_t)i;
            prime_index[i] = ++primes_cnt;
            primes[primes_cnt - 1] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            size_t ip = p * i;
            if (ip > SIEVE_LIMIT) break;
            if (spf[ip] == 0) spf[ip] = p;
            if (i % p == 0) break;
        }
    }

    /* ---------- Allocate sequence storage ---------- */
    seq_cap = 20000;                           /* initial capacity */
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;
}

/*--------------------------------------------------------------------
   Test whether v (≤ SIEVE_LIMIT) is a valid Heinz number according
   to the required partition condition.
--------------------------------------------------------------------*/
static bool is_valid_heinz(uint64_t v)
{
    if (v < 3u) return false;                 /* smallest term is 3 */

    uint32_t even_idx = 0;                    /* 0 means not yet found */
    int    exp_even  = 0;
    uint32_t min_odd_idx = UINT32_MAX;
    uint64_t t = v;

    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t idx = prime_index[p];        /* 1‑based index of prime p */

        if ((idx & 1u) == 0u) {               /* even‑indexed prime */
            if (even_idx != 0) return false;  /* more than one distinct even factor */
            even_idx = idx;
            int e = 0;
            while (t % p == 0u) {             /* count exponent of this prime */
                t /= p;
                ++e;
            }
            if (e > 1) return false;         /* even part repeated disallowed */
            exp_even = e;                    /* should be 1 */
        } else {                               /* odd‑indexed prime */
            while (t % p == 0u) t /= p;
            if (idx < min_odd_idx) min_odd_idx = idx;
        }
    }

    if (even_idx == 0) return false;           /* no even factor present */

    /* all odd‑indexed primes must have index > even_idx */
    if (min_odd_idx <= even_idx) return false;

    return true;
}

/*--------------------------------------------------------------------
   Ensure the cached sequence has at least n terms.
--------------------------------------------------------------------*/
static void ensure_n_terms(idx_t n)
{
    while (seq_len < n) {
        if (next_val > SIEVE_LIMIT) {
            /* In practice this should never happen for n ≤ 10000. */
            break;
        }
        if (is_valid_heinz((uint64_t)next_val)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)next_val;
        }
        ++next_val;
    }
}

/*--------------------------------------------------------------------
   Public function required by the grader.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init();
    ensure_n_terms(n);
    return seq[n - 1];
}
