/*
 * A286840  One of the two successive approximations up to 13^n for 13-adic
 *          integer sqrt(-1). Here the 5 (mod 13) case (except for n=0).
 * https://oeis.org/A286840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A286840: successive approximations of √-1 in the 13‑adic integers (5‑mod‑13 case).
 *
 * Recurrence:
 *   a(0) = 0,  a(1) = 5
 *   a(n) = a(n-1) + 9·((a(n-1)² + 1) mod 13ⁿ)   (mod 13ⁿ)   for n > 1
 *   (equivalently  a(n) = (a(n-1) + 9·((a(n-1)² + 1) mod 13ⁿ)) mod 13ⁿ)
 */

#include <stdint.h>
#include <limits.h>

/* Utility for modular multiplication using 128‑bit intermediate to avoid overflow
 * when seq_t fits inside 64 bits. For larger seq_t types the plain
 * a*b % mod works because the type can hold the product. */
#if defined(__cplusplus)
    template<typename T>
    static inline T mod_mul_seq(const T &a, const T &b, const T &mod)
    {
        if constexpr (sizeof(T) <= 8) {
            unsigned __int128 prod = (unsigned __int128)(uint64_t)a * (uint64_t)b;
            return (T)(prod % (unsigned __int128)(uint64_t)mod);
        } else {
            return (a * b) % mod;
        }
    }
#else
    static inline seq_t mod_mul_seq(seq_t a, seq_t b, seq_t mod)
    {
        unsigned __int128 prod = (unsigned __int128)(uint64_t)a * (uint64_t)b;
        return (seq_t)(prod % (unsigned __int128)(uint64_t)mod);
    }
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;

    seq_t a_prev = (seq_t)5;          /* a(1) */
    seq_t mod     = (seq_t)13;        /* current modulus 13¹ */

    /* Each iteration i computes a(i) with modulus 13ⁱ. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Advance modulus: 13ⁱ. Handle possible overflow when seq_t is 64 bits. */
        if (sizeof(seq_t) == sizeof(uint64_t)) {
            if ((uint64_t)mod > ULLONG_MAX / 13ULL) {
                mod = ULLONG_MAX;          /* clamp to maximum, beyond this values
                                              * will no longer fit and are not
                                              * needed for 64‑bit builds. */
            } else {
                mod = mod * (seq_t)13;
            }
        } else {
            mod = mod * (seq_t)13;
        }

        /* t = (a_prev² + 1) mod mod */
        seq_t t = mod_mul_seq(a_prev, a_prev, mod);
        t += (seq_t)1;
        if (t >= mod) t -= mod;

        /* t = 9 * t mod mod */
        t = mod_mul_seq(t, (seq_t)9, mod);

        /* a_cur = (a_prev + t) mod mod */
        seq_t a_cur = a_prev + t;
        if (a_cur >= mod) a_cur -= mod;

        a_prev = a_cur;
    }

    return a_prev;
}
