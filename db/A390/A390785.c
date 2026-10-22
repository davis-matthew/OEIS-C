/*
 * A390785  a(n) is the least k such that the number of integers between
 *          (1/4)*prime(k) and (1/4)*prime(k+1) is n.
 * https://oeis.org/A390785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390785
   a(n) = least k such that ⌊p_{k+1}/4⌋ - ⌊p_k/4⌋ = n,
   where p_k is the k‑th prime.  The function records the smallest
   k for each possible count and returns the stored value.
   It is built incrementally; any order of calls is accepted. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Prime generation (trial division) ---------------------------------*/
static uint64_t *pr = NULL;          /* primes array (first prime at index 0) */
static idx_t   pr_len = 0;           /* number of primes stored          */
static idx_t   pr_cap = 0;           /* capacity of the array            */
static uint64_t next_cand = 5;       /* next odd candidate to test       */

static void ensure_prime_idx(idx_t idx) {
    if (pr_len == 0) {                /* initialise with first two primes */
        pr_cap = 8;
        pr = (uint64_t*)malloc(pr_cap * sizeof(uint64_t));
        if (!pr) return;             /* out of memory – caller will fail */
        pr[0] = 2;
        pr[1] = 3;
        pr_len = 2;
    }
    while (pr_len <= idx) {           /* generate next prime              */
        uint64_t cand = next_cand;
        bool is_prime = true;

        /* trial division by known primes up to sqrt(cand) */
        for (idx_t i = 0; i < pr_len; ++i) {
            uint64_t p = pr[i];
            uint64_t pp = p * p;
            if (pp > cand) break;     /* p > sqrt(cand)   */
            if (cand % p == 0) {      /* divisible          */
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (pr_len == pr_cap) {
                pr_cap = pr_cap * 2 + 10;
                pr = (uint64_t*)realloc(pr, pr_cap * sizeof(uint64_t));
            }
            pr[pr_len++] = cand;
        }
        next_cand += 2;               /* skip even numbers          */
    }
}

/* ---- Mapping from count n to minimal k ---------------------------------*/
static seq_t *cnt2k = NULL;          /* cnt2k[n] = minimal k for count n */
static idx_t  cnt2k_sz = 0;          /* current size of cnt2k           */

/* grow cnt2k to hold indices up to 'need' (inclusive) */
static void ensure_cnt2k_size(idx_t need) {
    if (need + 1 > cnt2k_sz) {
        idx_t new_sz = (need + 1) * 2 + 10;
        seq_t *new_arr = (seq_t*)realloc(cnt2k, new_sz * sizeof(seq_t));
        if (!new_arr) return;           /* out of memory – caller will fail */
        /* initialise new elements to 0 */
        for (idx_t i = cnt2k_sz; i < new_sz; ++i) new_arr[i] = 0;
        cnt2k = new_arr;
        cnt2k_sz = new_sz;
    }
}

/* ---- Global state for incremental search --------------------------------*/
static idx_t next_k = 1;              /* next k to process, starting at 1 */

/* --------------------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n==0 */

    /* lazy allocation of cnt2k array */
    if (cnt2k == NULL) {
        cnt2k_sz = 128;
        cnt2k = (seq_t*)calloc(cnt2k_sz, sizeof(seq_t)); /* zeros */
    }

    /* Return cached result if present */
    if (cnt2k_sz > n && cnt2k[n] != 0)
        return cnt2k[n];

    /* Expand mapping array if needed */
    ensure_cnt2k_size(n);

    /* Process k values incrementally until answer found */
    for (;;) {
        /* Ensure we have primes p_k and p_{k+1} */
        ensure_prime_idx(next_k - 1);                 /* p_k   */
        ensure_prime_idx(next_k);                     /* p_{k+1} */

        /* Compute integer counts between ¼·p_k and ¼·p_{k+1} */
        seq_t low  = ((seq_t)pr[next_k - 1]) / (seq_t)4;
        seq_t high = (((seq_t)pr[next_k]) - (seq_t)1) / (seq_t)4;
        seq_t cnt_seq = high - low;
        idx_t cnt = (idx_t)cnt_seq;                  /* cast safe, cnt fits in idx_t */

        if (cnt > 0) {                               /* ignore empty gaps      */
            ensure_cnt2k_size(cnt);
            if (cnt2k[cnt] == 0)                     /* first occurrence only  */
                cnt2k[cnt] = (seq_t)next_k;
        }

        if (cnt == n)
            return cnt2k[n];

        ++next_k;
    }
}
