/*
 * A352911  Cantor's List: Pairs (i, j) of relatively prime positive integers
 *          sorted first by i + j then by i.
 * https://oeis.org/A352911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, look
 * Formula:   a(n) = the i‑th or j‑th component of the pair (i,j) of coprime
 *            positive integers sorted by i+j, then by i. The pairs are output
 *            as i followed by j
 * Range:     u8 n <= 9914, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static idx_t *pref = NULL;          /* pref[s] = Σ_{k=2}^{s} φ(k) */
static size_t pref_cap = 0;         /* allocated capacity for pref array */
static idx_t max_s = 1;             /* largest s for which pref is computed */

/* ensure pref has room for indices up to need (inclusive) */
static void ensure_pref_capacity(size_t need)
{
    if (pref_cap >= need + 1) return;

    size_t new_cap = pref_cap ? pref_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;

    pref = (idx_t *)realloc(pref, new_cap * sizeof(idx_t));
    pref_cap = new_cap;
}

/* compute φ(s) by testing gcd(i,s) for all 1 ≤ i < s */
static idx_t compute_phi(idx_t s)
{
    idx_t count = 0;
    for (idx_t i = 1; i < s; ++i)
        if (gcd((seq_t)i, (seq_t)s) == (seq_t)1)
            ++count;
    return count;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* safety, though n≥1 by definition */

    /* pair index (1‑based) */
    idx_t t = (n + 1) / 2;

    /* grow pref array until it covers pair index t */
    while (max_s < 2 || pref[max_s] < t) {
        ++max_s;
        ensure_pref_capacity(max_s);
        idx_t phi_val = compute_phi(max_s);
        if (max_s == 2)
            pref[2] = phi_val;
        else
            pref[max_s] = pref[max_s - 1] + phi_val;
    }

    /* now pref[max_s] ≥ t, so pair index t lies in the sum block s = max_s */
    idx_t s = max_s;
    idx_t prev_sum = (s == 2) ? 0 : pref[s - 1];
    idx_t offset = t - prev_sum - 1;   /* zero‑based offset within this block */

    /* find the offset‑th coprime i in 1..s-1 */
    idx_t i_candidate = 0;
    idx_t cnt = 0;
    for (idx_t i = 1; i < s; ++i) {
        if (gcd((seq_t)i, (seq_t)s) == (seq_t)1) {
            if (cnt == offset) {
                i_candidate = i;
                break;
            }
            ++cnt;
        }
    }

    /* output i or j depending on parity of n */
    if ((n & 1) == 1)  /* odd - return i */
        return (seq_t)i_candidate;
    else               /* even - return j = s - i */
        return (seq_t)(s - i_candidate);
}
