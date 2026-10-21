/*
 * A281717  Number of 3 X n 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          and with new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = 4, a(3) = 8, a(4) = 38, a(5) = 90, a(6) = 305,
 *            a(7) = 902, a(8) = 2710, a(9) = 8376, a(10) = 25226, a(11) =
 *            77145, a(12) = 235122, a(13) = 715882, a(14) = 2186483, a(15) =
 *            6666846, a(16) = 20353268, a(17) = 62140942, a(18) = 189739429,
 *            a(19) = 579567532, a(20) = 1770391431, a(21)...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------- Common includes ---------------------------- */
#include <stdlib.h>
#include <stdint.h>

/* ---------------------------- Constant values ---------------------------- */
static const seq_t init_vals[21] = {
    4ULL, 4ULL, 8ULL, 38ULL, 90ULL, 305ULL, 902ULL, 2710ULL, 8376ULL,
    25226ULL, 77145ULL, 235122ULL, 715882ULL, 2186483ULL, 6666846ULL,
    20353268ULL, 62140942ULL, 189739429ULL, 579567532ULL,
    1770391431ULL, 5409031357ULL
};

/* ---------------------------- C++ path --------------------------------- */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static idx_t cache_last = 0;

static void init_cache_cpp(void)
{
    if (cache_last >= 21) return;
    cache_vec.resize(22);           // indices 0..21, 0 unused
    for (idx_t i = 1; i <= 21; ++i) cache_vec[i] = init_vals[i-1];
    cache_last = 21;
}

static void compute_up_to_cpp(idx_t n)
{
    init_cache_cpp();
    for (idx_t k = cache_last + 1; k <= n; ++k)
    {
        seq_t sum = 0;
        sum += 3   * cache_vec[k-1];
        sum += 8   * cache_vec[k-2];
        sum -= 16  * cache_vec[k-3];
        sum -= 46  * cache_vec[k-4];
        sum += 39  * cache_vec[k-5];
        sum += 121 * cache_vec[k-6];
        sum -= 66  * cache_vec[k-7];
        sum -= 185 * cache_vec[k-8];
        sum += 78  * cache_vec[k-9];
        sum += 216 * cache_vec[k-10];
        sum -= 71  * cache_vec[k-11];
        sum -= 182 * cache_vec[k-12];
        sum += 51  * cache_vec[k-13];
        sum += 107 * cache_vec[k-14];
        sum -= 24  * cache_vec[k-15];
        sum -= 43  * cache_vec[k-16];
        sum += 7   * cache_vec[k-17];
        sum += 10  * cache_vec[k-18];
        sum -= cache_vec[k-19];
        sum -= cache_vec[k-20];
        cache_vec.push_back(sum);
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n <= 21) return init_vals[n-1];
    if (cache_last < n) compute_up_to_cpp(n);
    return cache_vec[n];
}

#else  /* ---------------------------- C path -------------------------------- */
static seq_t *cache = NULL;      /* dynamic array, cache[1] holds a(1) */
static idx_t cache_cap = 0;      /* capacity (indices 0..cache_cap-1) */
static idx_t cache_last = 0;     /* highest index that has been computed */

static void ensure_cache_c(idx_t n)
{
    if ((size_t)cache_cap >= (size_t)(n + 1)) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);
    cache = new_ptr;
    cache_cap = new_cap;
}

static void init_cache_c(void)
{
    if (cache_last >= 21) return;
    ensure_cache_c(21);
    for (idx_t i = 1; i <= 21; ++i) cache[i] = init_vals[i-1];
    cache_last = 21;
}

static void compute_up_to_c(idx_t n)
{
    init_cache_c();
    for (idx_t k = cache_last + 1; k <= n; ++k)
    {
        seq_t sum = 0;
        sum += 3   * cache[k-1];
        sum += 8   * cache[k-2];
        sum -= 16  * cache[k-3];
        sum -= 46  * cache[k-4];
        sum += 39  * cache[k-5];
        sum += 121 * cache[k-6];
        sum -= 66  * cache[k-7];
        sum -= 185 * cache[k-8];
        sum += 78  * cache[k-9];
        sum += 216 * cache[k-10];
        sum -= 71  * cache[k-11];
        sum -= 182 * cache[k-12];
        sum += 51  * cache[k-13];
        sum += 107 * cache[k-14];
        sum -= 24  * cache[k-15];
        sum -= 43  * cache[k-16];
        sum += 7   * cache[k-17];
        sum += 10  * cache[k-18];
        sum -= cache[k-19];
        sum -= cache[k-20];
        cache[k] = sum;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n <= 21) return init_vals[n-1];
    ensure_cache_c(n);
    if (cache_last < n) compute_up_to_c(n);
    return cache[n];
}
#endif
