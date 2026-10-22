/*
 * A306513  The number of unordered pairs of coprime integers q and r such
 *          that phi(q) + phi(r) = 2n.
 * https://oeis.org/A306513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered distinct coprime pairs {q,r} * such
 *            that φ(q)+φ(r)=2n, with q<r. * The implementation builds Euler’s
 *            totients up to a limit * that guarantees all numbers with φ ≤ 2n
 *            are included, * then counts the pairs
 * Range:     u8 n <= 36, u16 n <= 1150, u32 n <= 1150, u64 n <= 1150,
 *            u128 n <= 1150, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

typedef uint64_t idx_t;   /* alias for clarity – real build supplies this type */

/* ---------- helper data structures ---------- */

static uint32_t *phi      = NULL;   /* φ values for 1..φ_limit   */
static idx_t   **phi_list = NULL;   /* lists of numbers for each φ value (up to max_target) */
static size_t  *phi_cap   = NULL;   /* current capacity of each list          */
static size_t  *phi_cnt   = NULL;   /* current count   of each list           */

static idx_t   phi_limit      = 0;   /* maximum computed φ value */
static idx_t   max_target_val = 0;   /* maximum 2n that has been handled */

/* ---------- totient sieve & list construction ---------- */

static void ensure_phi(idx_t limit, idx_t max_target)
{
    if (limit <= phi_limit && max_target <= max_target_val) {
        return;   /* already sufficient */
    }

    /* free old data */
    if (phi) free(phi);
    if (phi_list) {
        for (idx_t i = 1; i <= max_target_val; ++i)
            if (phi_list[i]) free(phi_list[i]);
        free(phi_list);
        free(phi_cap);
        free(phi_cnt);
    }

    /* 1) compute totients up to 'limit' */
    phi = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    for (idx_t i = 1; i <= limit; ++i) phi[i] = (uint32_t)i;

    for (idx_t p = 2; p <= limit; ++p) {
        if (phi[p] == (uint32_t)p) {               /* p is prime */
            for (idx_t m = p; m <= limit; m += p)
                phi[m] -= phi[m] / (uint32_t)p;
        }
    }

    /* 2) allocate structures for φ values up to max_target */
    phi_list = (idx_t**)calloc((size_t)(max_target + 1), sizeof(idx_t*));
    phi_cap  = (size_t *)calloc((size_t)(max_target + 1), sizeof(size_t));
    phi_cnt  = (size_t *)calloc((size_t)(max_target + 1), sizeof(size_t));

    /* 3) populate lists with numbers whose φ ≤ max_target */
    for (idx_t i = 1; i <= limit; ++i) {
        uint32_t t = phi[i];
        if ((idx_t)t <= max_target) {
            if (phi_cnt[t] == phi_cap[t]) {
                size_t new_cap = (phi_cap[t] == 0) ? 4 : phi_cap[t] << 1;
                phi_cap[t] = new_cap;
                idx_t *tmp = (idx_t*)realloc(phi_list[t], new_cap * sizeof(idx_t));
                phi_list[t] = tmp;
            }
            phi_list[t][phi_cnt[t]++] = i;
        }
    }

    phi_limit      = limit;
    max_target_val = max_target;
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 – return 0 */

    idx_t target = 2 * n;            /* φ(q)+φ(r) = 2n */
    idx_t limit  = target * 10 + 20; /* generous limit: all numbers with φ≤target appear below this */

    /* recompute totients and lists if necessary */
    ensure_phi(limit, target);

    seq_t result = 0;

    for (idx_t t = 1; t < target; ++t) {
        idx_t t2 = target - t;
        if (t > t2) continue;                /* avoid double counting */

        size_t cnt1 = phi_cnt[t];
        size_t cnt2 = phi_cnt[t2];
        if (!cnt1 || !cnt2) continue;

        if (t == t2) {                       /* same φ value – need q<r */
            idx_t *dots = phi_list[t];
            for (size_t i = 0; i < cnt1; ++i) {
                idx_t q = dots[i];
                for (size_t j = i + 1; j < cnt1; ++j) {
                    idx_t r = dots[j];
                    if (gcd((seq_t)q, (seq_t)r) == (seq_t)1)
                        ++result;
                }
            }
        } else {                             /* distinct φ values */
            idx_t *L1 = phi_list[t];
            idx_t *L2 = phi_list[t2];
            for (size_t i = 0; i < cnt1; ++i) {
                idx_t q = L1[i];
                for (size_t j = 0; j < cnt2; ++j) {
                    idx_t r = L2[j];
                    if (gcd((seq_t)q, (seq_t)r) == (seq_t)1)
                        ++result;
                }
            }
        }
    }

    return result;
}
