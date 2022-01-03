<!-- Title -->
<span align = "center">

# PRN
  <b>ADC & Timer Configuration </b>

</span>
<!-- End of Title -->

### ADC Configuration
In this configuration, choose `ADC1` and check the ADC pin as below

| ADC PIN | Nomor Channel|
|---|---|
| IN0 | 0 |
| IN3 | 1 |
| IN4 | 2 |
| IN5 | 3 |
| IN8 | 4 |
| IN9 | 5 |

In the parameter setting, set as picture below

<span align="center">
  
 ![Logo](https://github.com/SuryaAssistant/prn_s2/blob/main/tutorial/tutorial_img/adc_config.png)
  
</span>

Open DMA Setting and click `Add` to add ADC1 into DMA Setting.

Set DMA Mode as Circular and set `Data Width` as `Word` both in peripheral and memory column.

<span align="center">
  
 ![Logo](https://github.com/SuryaAssistant/prn_s2/blob/main/tutorial/tutorial_img/dma_adc_config.png)
  
</span>

### Timer Configuration
For the timer configuration, set all timer's clock source as internal clock.

This Repository use 4 timer with different PWM frequency for all Timer.
The frequency is set by changing the prescaler and counter in every Timer.

| Timer | Prescaler | Counter | Frequency |
|---|---|
| TIM1 | 10-1 | 144 | 50 kHz |
| TIM2 | 8-1 | 600 | |
| TIM3 | 10-1 | 288 | 25 kHz |
| TIM4 | 72-1 | 2000 | 1 kHz |

Set the timer configuration as same as configuration in the table below.
For the Channel, set channel as `PWM Generation CHx`.

| Timer | Channel | Polarity |
|---|---|
| TIM1 | CH1 | LOW |
| TIM1 | CH2 | LOW |
| TIM1 | CH3 | HIGH |
| TIM2 | CH2 | LOW |
| TIM2 | CH3 | LOW |
| TIM3 | CH1 | LOW |
| TIM4 | CH1 | LOW |

### Frequency Calculation
```
Counter frequency = (required frequency * counter period)

Counter period = counter frequency / required frequency
```

```
Prescaler = (clock frequency / counter frequency) - 1
```

For example, maybe you need 25 kHz frequency. 
Let we say the counter frequency is 7.2 Mhz (you can change it as you want, but 7.2 will give easy calculation).
The counter period will be :

	counter period = 7.2Mhz / 25kHz
	counter period = 144

Then, calculate the prescaler

	prescaler = (72MHz / 7.2MHz) - 1
	prescaler = 10-1
