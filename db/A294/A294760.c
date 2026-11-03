/*
 * A294760  Number of permutations of [n] avoiding {4321, 1324, 1234}.
 * https://oeis.org/A294760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = number of permutations of [n] avoiding {4321, 1324, 1234}
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (By Erdos–Szekeres, a(n)=0 for n>=10. The rest is computed by brute force
   enumeration of all permutations up to size 9.) */
#include <stdbool.h>

/* Helper: test whether a permutation of length n avoids the three patterns. */
static bool _avoids(const int *perm, int n) {
    for (int a = 0; a < n - 3; ++a)
        for (int b = a + 1; b < n - 2; ++b)
            for (int c = b + 1; c < n - 1; ++c)
                for (int d = c + 1; d < n; ++d) {
                    int va = perm[a], vb = perm[b], vc = perm[c], vd = perm[d];
                    if ((va < vb && vb < vc && vc < vd)   /* 1234 */
                     || (va > vb && vb > vc && vc > vd)   /* 4321 */
                     || (va < vc && vc < vb && vb < vd))  /* 1324 */
                        return false;
                }
    return true;
}

/* Recursive generation of permutations of length "len". */
static void _gen(int len, int depth, int *perm, bool *used, seq_t *cnt) {
    if (depth == len) {
        if (_avoids(perm, len))
            ++(*cnt);
        return;
    }
    for (int i = 0; i < len; ++i) {
        if (!used[i]) {
            used[i] = true;
            perm[depth] = i + 1;    /* values 1..len */
            _gen(len, depth + 1, perm, used, cnt);
            used[i] = false;
        }
    }
}

/* Count avoiders for a given small len (0 <= len <= 9). */
static seq_t _count_avoiders(int len) {
    if (len == 0) return 1;
    seq_t cnt = 0;
    int perm[9];
    bool used[9] = {false};
    _gen(len, 0, perm, used, &cnt);
    return cnt;
}

/* Public entry function. */
seq_t a(idx_t n) {
    static bool ready = false;
    static seq_t cache[10];
    if (!ready) {
        /* Compute all values up to n = 9 once. */
        cache[0] = 1;                  /* by definition */
        for (int len = 1; len <= 9; ++len)
            cache[len] = _count_avoiders(len);
        ready = true;
    }
    if (n >= 10) return 0;            /* no avoiders for n >= 10 */
    return cache[(int)n];
}
