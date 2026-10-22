/*
 * A339819  Squarefree numbers k > 1 for which neither 2-adic nor 3-adic
 *          valuation of phi(k) exceeds the corresponding valuation of k-1.
 * https://oeis.org/A339819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8267, u32 n <= 8405, u64 n <= 8405,
 *            u128 n <= 8405, big any n
 * Helpers:   add_valutations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339819:
 *   Squarefree k > 1 such that v2(φ(k)) ≤ v2(k−1) and v3(φ(k)) ≤ v3(k−1).
 *   The function returns the n‑th term (1‑based index) by searching
 *   successive integers and counting the qualifying ones.  No dynamic
 *   allocation is required, so the code works both for fixed‑width
 *   seq_t and for arbitrary‑precision seq_t that can be cast to uint64_t
 *   for the range of n treated here. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/add_valutations.h"

/* test whether k is squarefree and whether v2(φ(k))≤v2(k-1) and
 * v3(φ(k))≤v3(k-1) for the case that k>1 and fits in uint64_t */
static bool good_candidate(uint64_t k)
{
    uint64_t km1 = k - 1;

    /* valuations of k-1 */
    int v2_km1 = 0, v3_km1 = 0;
    add_valutations(km1, &v2_km1, &v3_km1);

    /* valuations of φ(k): for squarefree k, φ(k)=∏(p-1) */
    int v2_phi = 0, v3_phi = 0;
    uint64_t n = k;

    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            /* check squarefree: d^2 must not divide n */
            if ((n / d) % d == 0) return false;
            /* accumulate valuations of (d-1) */
            add_valutations(d - 1, &v2_phi, &v3_phi);
            /* remove this distinct factor */
            n /= d;
        }
    }
    if (n > 1) { /* remaining prime factor */
        add_valutations(n - 1, &v2_phi, &v3_phi);
    }

    return (v2_phi <= v2_km1) && (v3_phi <= v3_km1);
}

/* cached search state – works for any seq_t that fits in uint64_t for the
 * indices considered (n up to ~10000).  No dynamic allocation needed. */
static uint64_t search_last_checked = 2;
static uint64_t search_count = 0;

/* public function required by the exercise */
seq_t a(idx_t n)
{
    /* undefined index – return 0 or any harmless value */
    if (n == 0) return 0;

    /* continue searching until we have at least n terms */
    while (search_count < (uint64_t)n) {
        uint64_t current = search_last_checked++;
        if (good_candidate(current)) {
            ++search_count;
        }
    }
    /* the n‑th term is the last found candidate */
    return (seq_t)(search_last_checked - 1);
}
