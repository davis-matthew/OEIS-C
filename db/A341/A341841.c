/*
 * A341841  Square array T(n, k), n, k >= 0, read by antidiagonals upwards;
 *          for any number m with runs in binary expansion (r_1, ..., r_j),
 *          let R(m) = {r_1 + ... + r_j, r_2 + ... + r_j, ..., r_j}; T(n, k)
 *          is the unique number t such that R(t) equals R(n) minus R(k).
 * https://oeis.org/A341841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, look
 * Formula:   a(n) is read by antidiagonals of the square table T(n,k), where
 *            T(n,k) is the unique integer t such that the set of suffix sums
 *            of the binary run‑lengths of t equals the set difference R(n) \
 *            R(k). R(m) is the set of partial sums of the run‑lengths of the
 *            binary expansion of m (R(0)=∅). This impl...
 * Range:     u8 n <= 9890, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   build_from_runs, run_and_suffix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_from_runs.h"
#include "../utils/run_and_suffix.h"

/* Check if v is contained in arr[0..len-1].  */
static bool contains(const idx_t arr[], size_t len, idx_t v)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == v) return true;
    return false;
}

/* Construct T(n,k) from the definition.  */
static seq_t T_from_nk(idx_t n, idx_t k)
{
    /* Runs and suffix sums for n.  */
    idx_t run_n[64], suf_n[64];
    size_t len_n_run, len_n_suf;
    run_and_suffix(n, run_n, &len_n_run, suf_n, &len_n_suf);

    /* Runs and suffix sums for k.  */
    idx_t run_k[64], suf_k[64];
    size_t len_k_run, len_k_suf;
    run_and_suffix(k, run_k, &len_k_run, suf_k, &len_k_suf);

    /* Set difference: R(n) \ R(k).  */
    idx_t diff[64];
    size_t len_diff = 0;
    for (size_t i = 0; i < len_n_suf; ++i)
        if (!contains(suf_k, len_k_suf, suf_n[i]))
            diff[len_diff++] = suf_n[i];
    if (len_diff == 0)
        return (seq_t)0;

    /* Sort diff descending (simple insertion sort).  */
    for (size_t i = 1; i < len_diff; ++i) {
        idx_t key = diff[i];
        size_t j = i;
        while (j > 0 && diff[j - 1] < key) {
            diff[j] = diff[j - 1];
            --j;
        }
        diff[j] = key;
    }

    /* Reconstruct run lengths from sorted suffix sums.  */
    idx_t run_diff[64];
    run_diff[len_diff - 1] = diff[len_diff - 1];
    for (size_t i = len_diff - 1; i > 0; --i)
        run_diff[i - 1] = diff[i - 1] - diff[i];

    /* Build integer from runs.  */
    idx_t val = build_from_runs(run_diff, len_diff);
    return (seq_t)val;
}

/* Main function requested by the evaluation system.  */
seq_t a(idx_t n)
{
    /* Map linear index n to coordinates (row, col) by antidiagonals
       upwards read.  */
    idx_t d = 0;                               /* diagonal counter */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    idx_t offset = n - d * (d + 1) / 2;         /* column position */
    idx_t row = d - offset;                    /* row position */
    idx_t col = offset;

    return T_from_nk(row, col);
}
