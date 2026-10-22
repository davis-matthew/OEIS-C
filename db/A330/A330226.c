/*
 * A330226  BII-numbers of fully chiral set-systems.
 * https://oeis.org/A330226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A330226
    Formula: a(n) = BII-number of the n-th fully chiral set–system
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Forward declaration of helper to check full chirality */
static bool is_fully_chiral(idx_t num);

/* Standard next_permutation implementation on an int array */
static bool next_permutation(int *a, int len)
{
    int i = len - 2;
    while (i >= 0 && a[i] > a[i + 1]) i--;
    if (i < 0) return false;
    int j = len - 1;
    while (a[j] < a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = len - 1;
    while (l < r) { tmp = a[l]; a[l] = a[r]; a[r] = tmp; l++; r--; }
    return true;
}

/* Checks whether a number “num” has a fully chiral set–system. */
static bool is_fully_chiral(idx_t num)
{
    /* ---------- 1. binary indices of n ---------- */
    int I[64];   /* positions of 1‑bits of num (1‑based) */
    int Icnt = 0;
    idx_t tmp = num;
    int pos = 1;
    while (tmp) {
        if (tmp & 1ULL) I[Icnt++] = pos;
        tmp >>= 1;
        pos++;
    }
    if (Icnt == 0) /* n = 0   -> empty set system */
        return true;

    /* ---------- 2. collect all vertices that belong to any set ---------- */
    int vertex_val[64];   /* actual vertex numbers (1‑based) */
    int vcnt = 0;
    int seen[65];         /* vertex numbers up to 64 are possible */
    memset(seen, 0, sizeof(seen));

    for (int idx = 0; idx < Icnt; idx++) {
        int i = I[idx];
        idx_t tt = i;
        int bitpos = 1;
        while (tt) {
            if (tt & 1ULL) {
                if (!seen[bitpos]) {
                    seen[bitpos] = 1;
                    vertex_val[vcnt++] = bitpos;
                }
            }
            tt >>= 1;
            bitpos++;
        }
    }

    /* mapping from vertex value to index (0 … vcnt-1) */
    int vmap[65];
    memset(vmap, -1, sizeof(vmap));
    for (int k = 0; k < vcnt; k++) vmap[vertex_val[k]] = k;

    /* ---------- 3. compute masks for each subset ---------- */
    int setcnt = Icnt;           /* each i in I gives one set */
    int setmask[64];
    for (int idx = 0; idx < setcnt; idx++) {
        int i = I[idx];
        idx_t tt = i;
        int bitpos = 1;
        int mask = 0;
        while (tt) {
            if (tt & 1ULL) {
                int vidx = vmap[bitpos];   /* vertex index within the set */
                mask |= 1 << vidx;
            }
            tt >>= 1;
            bitpos++;
        }
        setmask[idx] = mask;
    }

    /* ---------- 4. build original histogram of masks ---------- */
    int uniqMask[64], uniqCnt[64];
    int uniqsz = 0;
    for (int idx = 0; idx < setcnt; idx++) {
        int msk = setmask[idx];
        bool found = false;
        for (int j = 0; j < uniqsz; j++) {
            if (uniqMask[j] == msk) {
                uniqCnt[j]++;
                found = true;
                break;
            }
        }
        if (!found) {
            uniqMask[uniqsz] = msk;
            uniqCnt[uniqsz] = 1;
            uniqsz++;
        }
    }

    /* ---------- 5. test all non‑identity permutations ---------- */
    if (vcnt <= 1) return true;   /* no non‑trivial permutation possible */

    int perm[64];
    for (int k = 0; k < vcnt; k++) perm[k] = k;

    /* iterate over permutations after identity */
    while (next_permutation(perm, vcnt)) {
        /* build new histogram under this permutation */
        int newMask[64], newCnt[64];
        int newsz = 0;
        for (int idx = 0; idx < setcnt; idx++) {
            int msk = setmask[idx];
            int nmsk = 0;
            for (int b = 0; b < vcnt; b++) if (msk & (1 << b))
                nmsk |= 1 << perm[b];
            /* count nmsk */
            bool found = false;
            for (int j = 0; j < newsz; j++) {
                if (newMask[j] == nmsk) {
                    newCnt[j]++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                newMask[newsz] = nmsk;
                newCnt[newsz] = 1;
                newsz++;
            }
        }

        /* compare histograms */
        if (newsz != uniqsz) continue;
        bool equal = true;
        for (int i = 0; i < uniqsz; i++) {
            bool matched = false;
            for (int j = 0; j < newsz; j++) {
                if (uniqMask[i] == newMask[j]) {
                    if (uniqCnt[i] == newCnt[j]) matched = true;
                    break;
                }
            }
            if (!matched) { equal = false; break; }
        }
        if (equal) return false;   /* non‑trivial automorphism found */
    }
    return true;                  /* fully chiral */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t term = 1;        /* current position in the sequence (offset 1) */
    idx_t candidate = 0;   /* current BII‑number being tested */

    while (true) {
        if (is_fully_chiral(candidate)) {
            if (term == n) return (seq_t)candidate;
            term++;
        }
        candidate++;
    }
}
