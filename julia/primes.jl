function overestimate_pi_func(max_value)
    return convert(Int64, floor(max_value / (log(max_value) - 1.1)))
end

print(overestimate_pi_func(100))