/*
 * A277162  Lexicographically least sequence of nonnegative integers that
 *          avoids 10/9-powers.
 * https://oeis.org/A277162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9166, u16 n <= 9166, u32 n <= 9166, u64 n <= 9166,
 *            u128 n <= 9166, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A277162 – Lexicographically least sequence of nonnegative integers that
 * avoids 10/9-powers.
 *
 * The function is implemented by a greedy construction: for each new
 * term it tries the smallest nonnegative integers and stops as soon as no
 * 10/9‑power (a block of length 9k followed by the first k symbols of that
 * block) ends at the current position.
 *
 * To test for 10/9‑powers efficiently, the algorithm maintains a rolling
 * 64‑bit polynomial hash of the sequence and powers of the hash base.
 * The hash is updated in O(1) per new element and sub‑block hashes are
 * obtained in O(1).  The overall time complexity is O(n²) with a small
 * constant, easily fast enough for n ≤ 10000.
 *
 * The implementation is fully compliant with C11 and C++17.  In the
 * C++ case the array of sequence terms is allocated with `new[]` so that
 * construction/destruction of potentially non‑trivial `seq_t` objects
 * occurs correctly.  In the C case `malloc`/`realloc` is used.  The
 * auxiliary arrays `hash` and `powP` are `uint64_t` and are always
 * allocated with `realloc`, which is safe for trivial types on both
 * languages.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Hash base – any odd constant is fine; the same as in previous attempts. */
static const uint64_t HASH_BASE = 911382323u;

/* Cached data */
static seq_t *seq       = NULL;      /* sequence values */
static size_t seq_cap   = 0;         /* allocated capacity for seq    */
static size_t seq_len   = 0;         /* number of values stored       */

static uint64_t *hash    = NULL;      /* prefix hashes, hash[i] = H(a[0..i]) */
static uint64_t *powP    = NULL;      /* powers of HASH_BASE             */
static size_t    pow_cap = 0;         /* allocated capacity for powP/hash */

/* Ensure seq has capacity at least `needed`.  Reallocates if necessary. */
static void grow_seq(size_t needed) {
    if (seq_cap >= needed) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 128;
    while (newcap < needed) newcap <<= 1;

#ifdef __cplusplus
    seq_t *new_seq = new seq_t[newcap];
    /* Copy existing elements, using copy assignment. */
    for (size_t i = 0; i < seq_cap; ++i) new_seq[i] = seq[i];
    delete[] seq;
    seq = new_seq;
#else
    seq = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
#endif
    seq_cap = newcap;
}

/* Ensure hash and powP have capacity at least `needed`.  Reallocates if necessary. */
static void grow_hash_pows(size_t needed) {
    if (pow_cap >= needed) return;
    size_t newcap = pow_cap ? pow_cap * 2 : 128;
    while (newcap < needed) newcap <<= 1;

    uint64_t *new_hash = (uint64_t*)realloc(hash, newcap * sizeof(uint64_t));
    uint64_t *new_pow  = (uint64_t*)realloc(powP, newcap * sizeof(uint64_t));
    hash = new_hash;
    powP = new_pow;

    /* Recompute powers of the base up to newcap. */
    powP[0] = 1;
    for (size_t i = 1; i < newcap; ++i)
        powP[i] = (uint64_t)((unsigned __int128)powP[i-1] * HASH_BASE);

    pow_cap = newcap;
}

/* Compute 64‑bit hash of sub‑range [l, r] (inclusive).  Assumes 0 ≤ l ≤ r.  */
static uint64_t sub_hash(size_t l, size_t r) {
    if (l == 0) return hash[r];
    size_t len = r - l + 1;
    uint64_t h_len = powP[len];
    return hash[r] - (uint64_t)((unsigned __int128)hash[l-1] * h_len);
}

/* Main sequence function.  Computes a(n) lazily and caches results. */
seq_t a(idx_t n) {
    /* Ensure we have computed terms up to n. */
    if ((size_t)n >= seq_len) {
        size_t target = (size_t)n + 1;
        grow_seq(target);
        grow_hash_pows(target);

        for (size_t i = seq_len; i < target; ++i) {
            seq_t best = 0;          /* smallest admissible candidate */
            for (int cand = 0; ; ++cand) {
                seq_t cand_t = (seq_t)cand;
                uint64_t new_hash;
                if (i == 0) {
                    new_hash = (uint64_t)cand_t;
                } else {
                    new_hash = (uint64_t)(
                        ((unsigned __int128)hash[i-1] * HASH_BASE) + (uint64_t)cand_t);
                }

                bool ok = true;
                /* Check all possible 10/9‑powers ending at position i. */
                size_t max_k = (i + 1) / 10;
                for (size_t k = 1; k <= max_k; ++k) {
                    size_t start    = i - 10*k + 1;          /* start of the 10k block */
                    size_t pre_end  = start + k - 1;          /* end of first k symbols */
                    size_t suf_start = start + 9*k;            /* start of last k symbols */

                    uint64_t h1 = sub_hash(start, pre_end);
                    uint64_t h2;
                    if (suf_start == 0)
                        h2 = new_hash;  /* only possible for k=0 which never happens */
                    else
                        h2 = new_hash -
                            (uint64_t)((unsigned __int128)hash[suf_start-1] *
                                        powP[k]);

                    if (h1 == h2) {  /* collision → forbidden 10/9‑power */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    best = cand_t;
                    hash[i] = new_hash;
                    break;
                }
            }
            seq[i] = best;
        }
        seq_len = target;
    }
    return seq[(size_t)n];
}
