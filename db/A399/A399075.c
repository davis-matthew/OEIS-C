/*
 * A399075  G.f.: (Sum_{k>=1} x^(3*k)/(1 - x^(3*k))) * Product_{k>=1} (1 +
 *          x^k).
 * https://oeis.org/A399075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/3⌋} A000005(k) * A000009(n-3k)
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 195, u64 n <= 688,
 *            u128 n <= 2577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A000005(k) = number of divisors of k                */
/*   A000009(m) = number of partitions of m into distinct parts */
/* The implementation keeps two static tables that are extended
   lazily to the current requested n.  The tables are recomputed
   from scratch whenever a larger n is requested, ensuring the
   values are correct for any order of calls. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Offset 0 is defined as 0, all smaller values are 0 as well. */
    if (n == 0)
        return 0;

#ifdef __cplusplus
    /* --------- C++ implementation using std::vector ---------- */
    static std::vector<seq_t> part_dist;     /* A000009 values up to max_n   */
    static std::vector<seq_t> divisor_cnt;   /* d(k) values up to max_d       */
    static idx_t max_n = 0;                  /* largest n computed so far     */
    static idx_t max_d = 0;                  /* largest k (n/3) computed so far */

    idx_t new_n = n;
    idx_t new_d = n / 3;

    if (n > max_n) {
        /* Resize arrays to accommodate new maximum */
        part_dist.resize(new_n + 1);
        divisor_cnt.resize(new_d + 1);

        /* Zero all entries before recomputation */
        std::fill(part_dist.begin(), part_dist.end(), (seq_t)0);
        std::fill(divisor_cnt.begin(), divisor_cnt.end(), (seq_t)0);

        /* ---- Distinct-partitions: A000009 ---- */
        part_dist[0] = 1;
        for (idx_t k = 1; k <= new_n; ++k) {
            for (idx_t i = new_n; i >= k; --i) {
                part_dist[i] += part_dist[i - k];
            }
        }

        /* ---- Divisor counts: A000005 ---- */
        for (idx_t i = 1; i <= new_d; ++i) {
            for (idx_t j = i; j <= new_d; j += i) {
                ++divisor_cnt[j];
            }
        }

        max_n = new_n;
        max_d = new_d;
    }

    /* ---- Convolution to compute a(n) ---- */
    seq_t sum = 0;
    idx_t limit = n / 3;
    for (idx_t k = 1; k <= limit; ++k) {
        sum += divisor_cnt[k] * part_dist[n - 3 * k];
    }
    return sum;

#else
    /* --------- C implementation using raw arrays ---------- */
    static seq_t *part_dist = NULL; /* A000009 values */
    static seq_t *divisor_cnt = NULL; /* A000005 values */
    static idx_t max_n = 0;          /* largest n computed */
    static idx_t max_d = 0;          /* largest k (n/3) computed */

    idx_t new_n = n;
    idx_t new_d = n / 3;

    if (n > max_n) {
        /* Allocate zero‑initialized arrays */
        seq_t *new_part = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
        seq_t *new_div  = (seq_t *)calloc(new_d + 1, sizeof(seq_t));
        if (!new_part || !new_div) {
            free(new_part);
            free(new_div);
            return 0;          /* memory failure – unlikely on competitive platform */
        }

        /* ---- Distinct‑partitions: A000009 ---- */
        new_part[0] = 1;
        for (idx_t k = 1; k <= new_n; ++k) {
            for (idx_t i = new_n; i >= k; --i) {
                new_part[i] += new_part[i - k];
            }
        }

        /* ---- Divisor counts: A000005 ---- */
        for (idx_t i = 1; i <= new_d; ++i) {
            for (idx_t j = i; j <= new_d; j += i) {
                ++new_div[j];
            }
        }

        /* Replace old arrays (freeing only if they were previously allocated) */
        free(part_dist);
        free(divisor_cnt);
        part_dist = new_part;
        divisor_cnt = new_div;
        max_n = new_n;
        max_d = new_d;
    }

    /* ---- Convolution to compute a(n) ---- */
    seq_t sum = 0;
    idx_t limit = n / 3;
    for (idx_t k = 1; k <= limit; ++k) {
        sum += divisor_cnt[k] * part_dist[n - 3 * k];
    }
    return sum;
#endif
}
