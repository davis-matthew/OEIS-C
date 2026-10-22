/*
 * A330195  BII-number of the BII-normalization of the set-system with
 *          BII-number n.
 * https://oeis.org/A330195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest BII–number among all permutations of the vertex
 *            set of the set–system obtained by taking the binary indices of
 *            each binary index of n. (OEIS A330195)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool next_permutation(int *arr, int sz)
{
    int i = sz - 1;
    while (i > 0 && arr[i - 1] > arr[i]) i--;
    if (i == 0) return false;
    int j = sz - 1;
    while (arr[j] < arr[i - 1]) j--;
    int tmp = arr[i - 1];
    arr[i - 1] = arr[j];
    arr[j] = tmp;
    /* reverse tail */
    int l = i, r = sz - 1;
    while (l < r) {
        int t = arr[l];
        arr[l] = arr[r];
        arr[r] = t;
        l++; r--;
    }
    return true;
}

/* Compute a(n) in OEIS A330195. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* constants */
    const int MAX_BITS = 64;       /* maximum number of bits in n */
    const int MAX_SET_SIZE = 8;    /* maximum size of a set */

    int rawSets[MAX_BITS][MAX_SET_SIZE];
    int rawSetSizes[MAX_BITS];
    int setCount = 0;

    bool used[65] = {0};           /* vertex usage flags (1‑based) */
    /* --- step 1: build the raw set‑system --- */
    idx_t tmp = n;
    int pos = 1;                  /* bit position, 1‑based */
    while (tmp) {
        if (tmp & 1) {
            int sz = 0;
            idx_t tmpP = pos;
            int bitIdx = 1;
            while (tmpP) {
                if (tmpP & 1) {
                    rawSets[setCount][sz++] = bitIdx;
                    used[bitIdx] = true;
                }
                tmpP >>= 1;
                bitIdx++;
            }
            rawSetSizes[setCount] = sz;
            setCount++;
        }
        tmp >>= 1;
        pos++;
    }

    /* --- gather unique vertex labels --- */
    int vertexLabels[65];
    int vCnt = 0;
    for (int i = 1; i <= 64; i++)
        if (used[i]) vertexLabels[vCnt++] = i;

    /* --- map old vertex labels to 1..m --- */
    int newLabel[65] = {0};
    for (int i = 0; i < vCnt; i++)
        newLabel[(int)vertexLabels[i]] = i + 1;
    int m = vCnt;                 /* number of vertices (m <= 4) */

    /* --- normalize sets using the new labeling --- */
    int normMasks[MAX_BITS];
    for (int i = 0; i < setCount; i++) {
        int mask = 0;
        for (int j = 0; j < rawSetSizes[i]; j++) {
            int old = rawSets[i][j];
            int nl = newLabel[old];
            mask |= 1 << (nl - 1);
        }
        normMasks[i] = mask;
    }

    /* --- iterate over all permutations of the vertex set --- */
    int perm[4] = {0};            /* m <= 4 */
    for (int i = 0; i < m; i++) perm[i] = i + 1;

    seq_t best = 0;
    bool first = true;

    do {
        seq_t sum = 0;
        for (int i = 0; i < setCount; i++) {
            int origMask = normMasks[i];
            int transMask = 0;
            for (int v = 1; v <= m; v++) {
                if (origMask & (1 << (v - 1))) {
                    int newV = perm[v - 1];
                    transMask |= 1 << (newV - 1);
                }
            }
            int pX = transMask;                 /* integer whose binary indices give this set */
            seq_t contrib = ((seq_t)1 << (pX - 1));
            sum += contrib;
        }
        if (first || sum < best) {
            best = sum;
            first = false;
        }
    } while (next_permutation(perm, m));

    return best;
}
