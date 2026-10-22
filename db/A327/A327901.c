/*
 * A327901  Nonprime squarefree numbers whose prime indices all have the same
 *          sum of prime indices (A056239).
 * https://oeis.org/A327901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A327901: nonprime squarefree numbers whose prime
   indices all have the same sum of prime indices (A056239). */

/*  The algorithm builds a sieve up to a fixed limit, computes
    A056239 for all indices of primes up to that limit, and then
    generates the desired numbers by checking each integer for
    the required properties.  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 2000000ULL       /* sieve upper bound (adjustable) */

static uint32_t *spf = NULL;            /* smallest prime factor table     */
static uint32_t *prime_index = NULL;    /* index of prime p (1‑based)     */
static uint32_t *prime_list = NULL;     /* list of primes in order        */
static uint32_t *ap = NULL;             /* A056239 values for indices     */
static size_t   prime_cnt = 0;          /* number of primes <= LIMIT     */

static seq_t   *seq = NULL;             /* array of sequence values       */
static size_t   seq_len = 0;             /* number of terms stored         */
static idx_t    last_candidate = 1;      /* last integer examined          */

static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* allocate tables */
    spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    prime_index = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    prime_list = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));

    /* linear sieve to fill spf, prime_index, prime_list */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            prime_index[i] = (uint32_t)(prime_cnt + 1); /* 1‑based */
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            idx_t v = i * (idx_t)p;
            if (v > LIMIT) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }

    /* precompute A056239 for indices 0..prime_cnt */
    ap = (uint32_t *)malloc((prime_cnt + 1) * sizeof(uint32_t));
    ap[0] = 0;
    for (idx_t m = 2; m <= prime_cnt; ++m) {
        uint32_t t = (uint32_t)m;
        uint32_t sum = 0;
        while (t > 1) {
            uint32_t p = spf[t];
            uint32_t cnt = 0;
            while (t % p == 0) {
                t /= p;
                cnt++;
            }
            sum += cnt * prime_index[p];
        }
        ap[m] = sum;
    }

    /* initialise sequence with a(1)=1 */
    seq = (seq_t *)malloc(16 * sizeof(seq_t));
    seq_len = 1;
    seq[0] = (seq_t)1;
}

/* grow seq array if needed */
static void ensure_seq_size(size_t need)
{
    if (need <= 16) return; /* initial capacity */
    size_t new_cap = 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
}

/* test whether candidate n satisfies the conditions */
static bool good_candidate(idx_t n)
{
    if (n < 2) return true;          /* n==1 is accepted */
    if (spf[n] == n) return false;   /* prime, skip */

    uint32_t target_sum = 0;
    bool first = true;
    uint32_t t = (uint32_t)n;
    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        while (t % p == 0) {
            t /= p;
            e++;
            if (e > 1) return false;  /* not squarefree */
        }
        uint32_t idx = prime_index[p];          /* index of prime p */
        uint32_t s = ap[idx];                   /* A056239 of that index */
        if (first) {
            target_sum = s;
            first = false;
        } else if (s != target_sum) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    init();                       /* ensure tables exist */

    if (n == 0) return 0;         /* undefined for n < 1 */

    while (seq_len < (size_t)n) {
        ++last_candidate;         /* next integer to test */
        if (good_candidate(last_candidate)) {
            ensure_seq_size(seq_len + 1);
            seq[seq_len++] = (seq_t)last_candidate;
        }
    }
    return seq[n - 1];
}
