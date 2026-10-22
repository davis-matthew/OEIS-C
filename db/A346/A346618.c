/*
 * A346618  Triangle read by rows: T(n,k) = 1 iff 2 divides binomial(n,k) but
 *          4 does not (0 <= k <= n).
 * https://oeis.org/A346618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A346618 – Triangle read by rows: T(n,k) = 1 iff 2 divides binomial(n,k) but 4 does not
    a(n) is the sequence obtained by concatenating all rows T(0,:),T(1,:),….
    For each index n we locate its row r and column k and test
    whether v₂( binomial(r,k) ) == 1.
    Formula (in words): a(n) = 1 if popcount(k)+popcount(r−k)-popcount(r) == 1,
    otherwise 0, where r is the largest integer with r(r+1)/2 ≤ n and
    k = n − r(r+1)/2.  This uses Kummer’s theorem. */

#include <stdint.h>
#include <stdbool.h>

static idx_t find_row(idx_t n) {
    /* binary search for the maximum r with r*(r+1)/2 <= n */
    idx_t lo = 0;
    idx_t hi = n + 1;          /* r cannot exceed n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t r = find_row(n);
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    uint64_t nr = (uint64_t)r;
    uint64_t kr = (uint64_t)k;
    uint64_t nkr = nr - kr;

    uint64_t pc_n  = __builtin_popcountll(nr);
    uint64_t pc_k  = __builtin_popcountll(kr);
    uint64_t pc_nk = __builtin_popcountll(nkr);

    uint64_t v2 = pc_k + pc_nk - pc_n;

    return (seq_t)(v2 == 1 ? 1 : 0);
}
