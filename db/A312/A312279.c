/*
 * A312279  Coordination sequence Gal.4.59.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n > 13 Initial values a(0) … a(13)
 *            = {1,4,8,14,16,22,24,30,36,38,44,46,50,54}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#if defined(__cplusplus)
#include <vector>
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache;
    size_t nn = (size_t)n;

    if (cache.empty())
    {
        const seq_t init[14] = {1,4,8,14,16,22,24,30,36,38,44,46,50,54};
        cache.reserve(20000);                /* enough for expected n */
        for (size_t i = 0; i < 14; ++i)
            cache.push_back(init[i]);
    }

    if (nn >= cache.size())
    {
        size_t old = cache.size();
        cache.resize(nn + 1);
        for (size_t i = old; i <= nn; ++i)
            cache[i] = 2 * cache[i - 6] - cache[i - 12];
    }

    return cache[nn];
}
#else
static seq_t cache[20000];
static size_t cache_len = 0;
seq_t a(idx_t n)
{
    size_t nn = (size_t)n;

    if (cache_len == 0)
    {
        const seq_t init[14] = {1,4,8,14,16,22,24,30,36,38,44,46,50,54};
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init[i];
        cache_len = 14;
    }

    if (nn >= cache_len)
    {
        for (size_t i = cache_len; i <= nn; ++i)
            cache[i] = 2 * cache[i - 6] - cache[i - 12];
        cache_len = nn + 1;
    }

    return cache[nn];
}
#endif
