/*
 * A257703  Number of steps from n to 0 using this algorithm: x -> floor(e*x)
 *          if x is odd, and x -> floor(x/e) if x is even.
 * https://oeis.org/A257703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Number of steps from n to 0 using
   x -> floor(e*x) if x is odd,
   and    x -> floor(x/e) if x is even. */

/* 64‑bit constants for fixed‑point approximations of e and 1/e
   with 15 fractional digits.  The errors are <5·10⁻¹⁶,
   which is far smaller than 1 for all x that appear when n ≤ 10⁴ */
/*  e  ≈ 2.718281828459045   -> 2718281828459045 / 10¹⁵ */
static const uint64_t E_NUM  = 2718281828459045ULL;
static const uint64_t E_DEN  = 1000000000000000ULL;
/* 1/e ≈ 0.367879441171442   -> 367879441171442  / 10¹⁵ */
static const uint64_t INV_E_NUM = 367879441171442ULL;
static const uint64_t INV_E_DEN = 1000000000000000ULL;

seq_t a(idx_t n)
{
    /* step counter */
    seq_t steps = 0;
    /* current value, promoted to the generic type */
    seq_t x = (seq_t)n;

    /* iterate until the value reaches zero */
    while (x != 0)
    {
        /* if odd → floor(e * x) */
        if (x & 1U)
        {
            /* use 128‑bit intermediate to avoid overflow */
            x = (seq_t)(((__uint128_t)x * E_NUM) / E_DEN);
        }
        /* else even → floor(x / e) */
        else
        {
            x = (seq_t)(((__uint128_t)x * INV_E_NUM) / INV_E_DEN);
        }
        ++steps;
    }

    return steps;
}
