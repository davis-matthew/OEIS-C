/*
 * A270303  Numbers which are representable as a sum of nineteen but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique integer n which is a multiple of 19 and can
 *            be written as the sum of 19 consecutive non‑negative integers,
 *            but not as the sum of k consecutive non‑negative integers for
 *            any 2 ≤ k ≤ 18. The sequence starts with a(1)=304
 * Range:     u8 none, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm tests successive multiples of 19.
    For each candidate m (so that n = 19*m) it checks
    the congruence conditions derived from the
    necessary and sufficient representation test:
        k odd :   n ≡ 0      (mod k)
        k even:  n ≡ k/2    (mod k)
    For an odd k this reduces to m ≠ 0 (mod k)
    (since 19 is coprime to all odd k < 19).
    For an even k it reduces to
        m ≠ (k/2) * inv19 (mod k),
    where inv19 is the modular inverse of 19 modulo k.
    All forbidden residues are pre‑computed once.
    The first term for which all forbidden residues are avoided
    is stored; the list is built on-demand, allowing calls in any
    order.  */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of found terms   */
static size_t cache_cap = 0;         /* current capacity               */
static size_t cache_len = 0;         /* number of terms stored         */
static idx_t  m_next = 9;            /* next candidate m to test (≥9) */

/* Forbidden residues for even k: r_even[k] = (k/2) * inv19 (mod k)  */
static int r_even[19] = {0};         /* indices are the values of k   */

/* Pre‑compute modular inverses of 19 modulo even k (2≤k≤18).   */
static void init_residues(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    for (int k = 2; k <= 18; k += 2) {
        int inv = 1;
        /* brute force inverse since k ≤ 18 */
        for (int x = 1; x < k; ++x)
            if ((19 * x) % k == 1) { inv = x; break; }
        r_even[k] = ((k / 2) * inv) % k;
    }
}

/* Make sure the cache has room for at least 'needed' more entries. */
static void ensure_capacity(size_t needed)
{
    if (cache_len + needed <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    while (cache_len + needed > new_cap) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Test whether candidate m satisfies all congruence constraints. */
static int is_valid_m(idx_t m)
{
    /* Odd k:  3,5,7,9,11,13,15,17   -> m mod k ≠ 0     */
    for (int k = 3; k <= 17; k += 2)
        if ((m % k) == 0) return 0;

    /* Even k: 2,4,6,8,10,12,14,16,18  -> m mod k ≠ r_even[k] */
    for (int k = 2; k <= 18; k += 2)
        if ((m % k) == r_even[k]) return 0;

    return 1;
}

/*--------------------------------------------------------------------
   Public function according to the required prototype. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, ignore 0          */

    init_residues();

    while (cache_len < n) {
        /* search for the next valid m */
        while (!is_valid_m(m_next)) m_next++;
        /* store the corresponding term a = 19*m */
        ensure_capacity(1);
        cache[cache_len++] = (seq_t)19 * (seq_t)m_next;
        m_next++;                      /* next candidate for future calls*/
    }
    return cache[n - 1];
}
