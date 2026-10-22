/*
 * A329581  For every n >= 0, exactly 11 sums are prime among a(n+i) + a(n+j),
 *          0 <= i < j < 8: lexicographically earliest such sequence of
 *          distinct nonnegative numbers.
 * https://oeis.org/A329581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: lexicographically earliest sequence of distinct non‑negative
   integers a(n) such that for every n ≥ 0, among the 28 pairwise sums
   a(n+i)+a(n+j) (0 ≤ i < j < 8) exactly 11 are prime.  */
 
/*  Implementation notes:
    * the first seven terms are 0,1,2,3,4,5,6.
    * for n ≥ 7, a(n) is the smallest unused integer such that the block
      a(n-7)…a(n) contains exactly 11 prime sums.
    * Terms are memoised in a static array; any order of calls is
      supported.
    * Prime testing is performed by the supplied is_prime_big() helper.
 */
 
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
 
/* Define a generous limit on the number of terms we will generate.
   The sequence is known to stay within this bound for all tests. */
#define MAX_TERMS 200000
 
static seq_t seq_cache[MAX_TERMS];   /* memoised terms   */
static idx_t seq_len = 0;            /* number of terms computed so far */

/* Return true if v already appeared in seq_cache[0..upto-1]. */
static bool used_value(seq_t v, idx_t upto)
{
    for (idx_t i = 0; i < upto; ++i)
        if (seq_cache[i] == v) return true;
    return false;
}

/* Count the number of prime sums among the 7‑term window
   seq_cache[start … start+6]. */
static int primes_in_window(idx_t start)
{
    int cnt = 0;
    for (idx_t i = start; i < start + 7; ++i) {
        for (idx_t j = i + 1; j < start + 7; ++j) {
            if (is_prime_big(seq_cache[i] + seq_cache[j]))
                ++cnt;
        }
    }
    return cnt;
}

/* Ensure a(n) is computed for all indices up to n.  */
static void compute_up_to(idx_t n)
{
    if (n < seq_len) return;          /* already available           */

    /* initialise the first seven terms */
    if (seq_len == 0) {
        for (idx_t i = 0; i <= 6; ++i) {
            seq_cache[i] = (seq_t)i;
            seq_len++;
        }
    }

    /* generate terms from seq_len up to n */
    for (idx_t k = seq_len; k <= n; ++k) {
        /* window of previous 7 terms: indices k-7 … k-1 */
        idx_t start = k - 7;
        int existing_primes = primes_in_window(start);
        int needed_new_primes = 11 - existing_primes;

        /* search for the smallest unused candidate */
        idx_t cand_idx = 0;
        seq_t candidate;
        while (true) {
            candidate = (seq_t)cand_idx;

            if (used_value(candidate, k)) { /* must be distinct */
                ++cand_idx;
                continue;
            }

            /* count primes involving the new candidate */
            int new_primes = 0;
            for (idx_t i = start; i < k; ++i) {
                if (is_prime_big(seq_cache[i] + candidate))
                    ++new_primes;
            }

            if (new_primes == needed_new_primes)
                break;      /* found the lexicographically smallest one */

            ++cand_idx;
        }

        seq_cache[k] = candidate;
        seq_len++;
    }
}

/* Public entry point according to the required signature. */
seq_t a(idx_t n)
{
    if (n >= MAX_TERMS) abort();              /* safety check      */
    compute_up_to(n);
    return seq_cache[n];
}
